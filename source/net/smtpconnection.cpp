//
// Created by Wihy on 12/28/25.
//

#include "../../headers/net/smtpconnection.h"

#include <iostream>
#include <qhostaddress.h>

SmtpConnection::SmtpConnection(const QString &from, const QString &to, const QString &subject,
                               const QString &body) : connection(this) {
    connect(&connection, &QTcpSocket::readyRead, this, &SmtpConnection::ready_read);
    connect(&connection, &QAbstractSocket::errorOccurred, this, &SmtpConnection::handle_error);

    message = "To: " + to + "\n";
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append(body);
    message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    this->from = from;
    recipient = to;
    state = State::INIT;

    connection.abort();
    connection.connectToHost(QHostAddress("192.168.1.156"), 2500); //todo pi.

    if (connection.waitForConnected(30000)) { qDebug("Connected to SMTP server"); }
}

SmtpConnection::~SmtpConnection() = default;

void SmtpConnection::ready_read() {
    QString responseLine;
    do {
        responseLine = connection.readLine();
        response += responseLine;
    } while (connection.canReadLine() && responseLine.length() > 3 && responseLine[3] != ' ');

    const QString code = responseLine.left(3);

    if (state == State::INIT && code.startsWith('2')) {
        connection.write("HELO there\r\n");
        state = State::MAIL;
    } else if (state == State::MAIL && code.startsWith('2')) {
        connection.write(QString("MAIL FROM: <%1>\r\n").arg(from).toLatin1());
        state = State::RCPT;
    } else if (state == State::RCPT && code.startsWith('2')) {
        connection.write(QString("RCPT TO: <%1>\r\n").arg(recipient).toLatin1());
        state = State::DATA;
    } else if (state == State::DATA && code.startsWith('2')) {
        connection.write("DATA\r\n");
        state = State::BODY;
    } else if (state == State::BODY && code.startsWith('3')) {
        // Sending the actual message body followed by the end-of-data period
        connection.write(message.toLatin1() + "\r\n.\r\n");
        state = State::QUIT;
    } else if (state == State::QUIT && code.startsWith('2')) {
        connection.write("QUIT\r\n");
        state = State::CLOSE;
        emit status(tr("Message sent"));
    } else if (state == State::CLOSE) {
        deleteLater();
        return;
    } else {
        QMessageBox::warning(nullptr, tr("Qt Mail "), tr("Unexpected reply from SMTP server:\n\n") + response);
        state = State::CLOSE;
    }

    response = "";
}

void SmtpConnection::handle_error(const QAbstractSocket::SocketError socketError) {
    const char *errorMessage = nullptr;

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

bool SmtpConnection::is_connected() const {
    return connection.state() == QTcpSocket::ConnectedState;
}
