//
// Created by Wihy on 12/28/25.
//

#include "../../headers/net/smtpconnection.h"

#include <iostream>
#include <qhostaddress.h>
#include <QSslConfiguration>

#include "../../headers/globals.h"
#include "../../headers/net/commands.h"

SmtpConnection::SmtpConnection(const QString &from, const QString &to, const QString &subject,
                               const QString &body) : connection(this) {
    connect(&connection, &QTcpSocket::readyRead, this, &SmtpConnection::ready_read);
    connect(&connection, &QAbstractSocket::errorOccurred, this, &SmtpConnection::handle_error);

    message = "To: " + to + "\r\n";
    message.append("From: " + from + "\r\n");
    message.append("Subject: " + subject + "\r\n");
    message.append("\r\n");
    message.append(body);

    message.replace("\n.", "\n..");

    message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\r\n"), QString::fromLatin1("\r\n"));

    this->from = from;
    recipient = to;
    state = State::INIT;

    const QSslConfiguration config = connection.sslConfiguration();
    connection.setSslConfiguration(config);
    connection.setPeerVerifyMode(QSslSocket::VerifyNone);

    connection.connectToHost(QHostAddress("192.168.1.156"), 2500); //todo pi.

    if (connection.waitForConnected(30000)) { qDebug("Connected to SMTP server"); }

    connect(&connection, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(handle_ssl_errors(QList<QSslError>)));
}

SmtpConnection::~SmtpConnection() = default;

void SmtpConnection::ready_read() {
    while (connection.canReadLine()) {
        QString line = connection.readLine().trimmed();
        QString code = line.left(3);

        if (line.length() >= 4 && line[3] == '-') {
            continue;
        }

        if (state == State::INIT && code == "220") {
            connection.write("EHLO RoyaleClient\r\n");
            state = State::TLS_REQ;
        } else if (state == State::TLS_REQ && code == "250") {
            connection.write("STARTTLS\r\n");
            state = State::TLS_START;
        } else if (state == State::TLS_START && code == "220") {
            connection.startClientEncryption();
            connection.write("EHLO RoyaleClient\r\n");
            state = State::AUTH_REQ;
        } else if (state == State::AUTH_REQ && code == "250") {
            connection.write("AUTH LOGIN\r\n");
            state = State::AUTH_USER;
        } else if (state == State::AUTH_USER && code == "334") {
            connection.write(CLIENT_NAME.toUtf8().toBase64() + "\r\n");
            state = State::AUTH_PASS;
        } else if (state == State::AUTH_PASS && code == "334") {
            connection.write(QString::fromStdString(hash(RAW_PASSWORD.toStdString())).toUtf8().toBase64() + "\r\n");
            state = State::MAIL;
        } else if (state == State::MAIL && code == "235") {
            connection.write(QString("MAIL FROM: <%1>\r\n").arg(from).toUtf8());
            state = State::RCPT;
        } else if (state == State::RCPT && code == "250") {
            connection.write(QString("RCPT TO: <%1>\r\n").arg(recipient).toUtf8());
            state = State::DATA;
        } else if (state == State::DATA && code == "250") {
            connection.write("DATA\r\n");
            state = State::BODY;
        } else if (state == State::BODY && code == "354") {
            connection.write(message.toUtf8() + "\r\n.\r\n");
            state = State::QUIT;
        } else if (state == State::QUIT && code == "250") {
            connection.write("QUIT\r\n");
            state = State::CLOSE;
            emit status(tr("Message sent"));
        } else if (state == State::CLOSE || code == "221") {
            connection.close();
            deleteLater();
        } else {
            qWarning() << "SMTP Error. State:" << static_cast<int>(state) << "Response:" << line;
            state = State::CLOSE;
            connection.close();
        }
    }
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
void SmtpConnection::handle_ssl_errors(const QList<QSslError> &errors) const {
    for (const auto &error : errors) {
        qWarning() << "[SSL Error]:" << error.errorString();
    }

    qobject_cast<QSslSocket*>(sender())->ignoreSslErrors();
}

bool SmtpConnection::is_connected() const {
    return connection.state() == QTcpSocket::ConnectedState;
}
