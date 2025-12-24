#include "../../headers/net/remotepi.h"
#include "../../headers/net/conversions.h"
#include <iostream>
#include <QtWidgets>
#include <QtNetwork>

RemotePi::RemotePi() : connection(this) {
    connect(&connection, &QAbstractSocket::errorOccurred, this, &RemotePi::print_error);
    connect(&connection, &QTcpSocket::readyRead, this, &RemotePi::read_from_pi);
}

bool RemotePi::is_connected() const {
    return connection.state() == QTcpSocket::ConnectedState;
}

bool RemotePi::send_cmd_to_pi(const Command cmd_type, const QStringList& parameters) {
    if (!is_connected()) return false;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    stream << quint32(0);
    stream << quint32(cmd_type);

    for (const QString& parameter : parameters) {
        stream << parameter;
    }

    const quint32 payload_size = data.size() - sizeof(quint32);

    stream.device()->seek(0);
    stream << payload_size;

    connection.write(data);
    return true;
}

QString RemotePi::read_from_pi() { //TODO: Implement properly. error codes and such! server will send success/failure.
    if (connection.bytesAvailable() == 0) return "";

    qDebug() << "Server replied: " << connection.readAll();
    return " ";
}

void RemotePi::connect_to_pi() {
    std::cout << "Connecting to PI..." << std::endl;
    connection.abort();
    connection.connectToHost(QHostAddress("192.168.1.156"), 5004);
 }

bool RemotePi::sign_up(const QString &name, const QString &password) {
    QString new_name = QString("jeff");
    QString new_password = QString("peesta");
    //todo remove

    return send_cmd_to_pi(SIGN_UP, {new_name, new_password});
}

bool RemotePi::log_in(QString name, QString password) {
    //todo: Impl
    return false;
}

void RemotePi::print_error(const QAbstractSocket::SocketError socketError) {
    std::cout << "Error: " << socketError;
}
