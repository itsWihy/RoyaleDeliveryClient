#include "../../headers/net/remotepi.h"

#include <iostream>
#include <QtNetwork>
#include <QtWidgets>

#include "../../headers/windows/signupwindow.h"


RemotePi::RemotePi() : connection(this) {
    connect(&connection, &QAbstractSocket::errorOccurred, this, &RemotePi::handle_error);
    connect(&connection, &QTcpSocket::readyRead, this, &RemotePi::handle_server_data);
}

bool RemotePi::is_connected() const {
    return connection.state() == QTcpSocket::ConnectedState;
}

bool RemotePi::send_cmd_to_server(const Command cmd_type, const QStringList& parameters) {
    if (!is_connected()) return false;
    const QByteArray data{pack_data(cmd_type, parameters)};

    return connection.write(data) >= 0;
}

void RemotePi::handle_server_data() const {
    auto *socket = qobject_cast<QTcpSocket *>(sender());

    QDataStream stream(socket->readAll());
    stream.setVersion(QDataStream::Qt_5_15);

    stream.startTransaction();

    quint32 length, cmd_type;
    stream >> length >> cmd_type;

    if (!stream.commitTransaction()) return;

    switch (cmd_type) {
        case STATUS: {
            QString type, result;
            stream >> type >> result;

            if (type == "SIGNUP")
                result.prepend("SIGNUP");

            if (type == "LOGIN")
                result.prepend("LOGIN");

            emit server_message_received(STATUS, result);

            break;
        }
    }
}

void RemotePi::connect_to_pi() {
    std::cout << "Connecting to PI..." << std::endl;
    connection.abort();
    connection.connectToHost(QHostAddress("192.168.1.156"), 5004);
 }

bool RemotePi::sign_up(const QString &name, const QString &password) {
    return send_cmd_to_server(SIGN_UP, {name, password});
}

bool RemotePi::log_in(const QString& name, const QString& password) {
    return send_cmd_to_server(LOG_IN, {name, password});
}

void RemotePi::handle_error(const QAbstractSocket::SocketError socketError) {
    const char* errorMessage = nullptr;

    switch (socketError) {
        case QAbstractSocket::ConnectionRefusedError:
            errorMessage = "Connection refused by the peer";
            break;
        case QAbstractSocket::RemoteHostClosedError:
            errorMessage = "Remote host closed the connection";
            break;
        case QAbstractSocket::HostNotFoundError:
            errorMessage = "Host not found";
            break;
        case QAbstractSocket::SocketAccessError:
            errorMessage = "Socket access error (permission issue)";
            break;
        case QAbstractSocket::SocketTimeoutError:
            errorMessage = "Socket operation timed out";
            break;
        case QAbstractSocket::NetworkError:
            errorMessage = "Network error";
            break;
        default:
            errorMessage = "Unknown socket error";
            break;
    }

    std::cerr << "[Socket Error] Code: " << socketError << ", Message: " << errorMessage << std::endl;
}
