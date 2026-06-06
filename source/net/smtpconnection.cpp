//
// Created by Wihy on 12/28/25.
//

#include "../../headers/net/smtpconnection.h"

#include <iostream>
#include <qhostaddress.h>
#include <QSslConfiguration>

#include "../../headers/globals.h"
#include "../../headers/net/commands.h"

/**
 * @file smtpconnection.cpp
 * @brief Implementation of the SMTP protocol client for sending emails.
 */

#include "../../headers/net/smtpconnection.h"

#include <iostream>
#include <qhostaddress.h>
#include <QSslConfiguration>

#include "../../headers/globals.h"
#include "../../headers/net/commands.h"

/**
 * @brief Constructs an SMTP connection and prepares the email message.
 * 
 * Formats the email headers (To, From, Subject) and body according to RFC standards,
 * then initiates a connection to the SMTP server.
 * 
 * @param from Sender email address.
 * @param to Recipient email address.
 * @param subject Email subject line.
 * @param body Email body content.
 */
SmtpConnection::SmtpConnection(const QString &from, const QString &to, const QString &subject,
                               const QString &body) : connection(this) {
    // Connect networking and SSL signals
    connect(&connection, &QTcpSocket::readyRead, this, &SmtpConnection::ready_read);
    connect(&connection, &QAbstractSocket::errorOccurred, this, &SmtpConnection::handle_error);
    connect(&connection, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(handle_ssl_errors(QList<QSslError>)));

    // Prepare the email message with CRLF line endings
    message = "To: " + to + "\r\n";
    message.append("From: " + from + "\r\n");
    message.append("Subject: " + subject + "\r\n");
    message.append("\r\n");
    message.append(body);

    // Escape dots at the beginning of lines (data transparency)
    message.replace("\n.", "\n..");
    // Ensure all newlines are \r\n
    message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\r\n"), QString::fromLatin1("\r\n"));

    this->from = from;
    recipient = to;
    state = State::INIT;

    // For this prototype, we ignore SSL verification for simplicity
    connection.setPeerVerifyMode(QSslSocket::VerifyNone);
    connection.ignoreSslErrors();

    // Connect to the local SMTP server port
    connection.connectToHost(QHostAddress(PI_ADDRESS), 2500);
}

/**
 * @brief Default destructor.
 */
SmtpConnection::~SmtpConnection() = default;

/**
 * @brief Core SMTP state machine.
 * 
 * Triggered whenever the server sends a response. Parses the 3-digit SMTP code
 * and sends the next command in the sequence (EHLO -> STARTTLS -> AUTH -> MAIL FROM -> RCPT TO -> DATA -> QUIT).
 */
void SmtpConnection::ready_read() {
    while (connection.canReadLine()) {
        QString line = connection.readLine().trimmed();
        QString code = line.left(3);

        // Multi-line responses (ending in '-') are ignored until the last line (ending in ' ')
        if (line.length() >= 4 && line[3] == '-') {
            continue;
        }

        if (state == State::INIT && code == "220") {
            // Server ready, say hello
            connection.write("EHLO RoyaleClient\r\n");
            state = State::TLS_REQ;
        } else if (state == State::TLS_REQ && code == "250") {
            // Request upgrade to secure connection
            connection.write("STARTTLS\r\n");
            state = State::TLS_START;
        } else if (state == State::TLS_START && code == "220") {
            // Start the SSL/TLS handshake
            connection.startClientEncryption();
            // Re-identify after upgrading to TLS
            connection.write("EHLO RoyaleClient\r\n");
            state = State::AUTH_REQ;
        } else if (state == State::AUTH_REQ && code == "250") {
            // Request login authentication
            connection.write("AUTH LOGIN\r\n");
            state = State::AUTH_USER;
        } else if (state == State::AUTH_USER && code == "334") {
            // Send base64-encoded username
            connection.write(CLIENT_NAME.toUtf8().toBase64() + "\r\n");
            state = State::AUTH_PASS;
        } else if (state == State::AUTH_PASS && code == "334") {
            // Send base64-encoded hashed password
            connection.write(QString::fromStdString(hash(RAW_PASSWORD.toStdString())).toUtf8().toBase64() + "\r\n");
            state = State::MAIL;
        } else if (state == State::MAIL && code == "235") {
            // Authentication successful, specify sender
            connection.write(QString("MAIL FROM: <%1>\r\n").arg(from).toUtf8());
            state = State::RCPT;
        } else if (state == State::RCPT && code == "250") {
            // Specify recipient
            connection.write(QString("RCPT TO: <%1>\r\n").arg(recipient).toUtf8());
            state = State::DATA;
        } else if (state == State::DATA && code == "250") {
            // Request to send data
            connection.write("DATA\r\n");
            state = State::BODY;
        } else if (state == State::BODY && code == "354") {
            // Send the actual email body followed by the end-of-data sequence
            connection.write(message.toUtf8() + "\r\n.\r\n");
            state = State::QUIT;
        } else if (state == State::QUIT && code == "250") {
            // Transmission finished, quit
            connection.write("QUIT\r\n");
            state = State::CLOSE;
            emit status(tr("Message sent"));
        } else if (state == State::CLOSE || code == "221") {
            // Final goodbye
            connection.close();
            deleteLater(); // Auto-destruct after session
        } else {
            // Log unexpected response codes and abort
            qWarning() << "SMTP Error. State:" << static_cast<int>(state) << "Response:" << line;
            state = State::CLOSE;
            connection.close();
        }
    }
}

/**
 * @brief Error handler for socket failures.
 */
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

/**
 * @brief Logs SSL errors encountered during handshake.
 */
void SmtpConnection::handle_ssl_errors(const QList<QSslError> &errors) const {
    for (const auto &error : errors)
        qWarning() << "[SSL Error ignored]:" << error.errorString();
}

/**
 * @brief Checks if the underlying socket is connected.
 */
bool SmtpConnection::is_connected() const {
    return connection.state() == QTcpSocket::ConnectedState;
}
