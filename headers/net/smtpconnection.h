//
// Created by Wihy on 12/28/25.
//

/**
 * @file smtpconnection.h
 * @brief Handles SMTP protocol for sending emails over SSL/TLS.
 */

#ifndef ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
#define ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
#include <QObject>
#include <qobjectdefs.h>

#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

/**
 * @brief Enum representing the states of the SMTP protocol finite state machine.
 */
enum class State {
    INIT,       ///< Initial connection state.
    TLS_REQ,    ///< Requesting STARTTLS.
    TLS_START,  ///< Negotiating SSL/TLS handshake.
    AUTH_REQ,   ///< Requesting authentication (LOGIN).
    AUTH_USER,  ///< Sending username.
    AUTH_PASS,  ///< Sending password.
    MAIL,       ///< Sending MAIL FROM command.
    RCPT,       ///< Sending RCPT TO command.
    DATA,       ///< Sending DATA command.
    BODY,       ///< Sending email content.
    QUIT,       ///< Sending QUIT command.
    CLOSE       ///< Closing the connection.
};

/**
 * @class SmtpConnection
 * @brief Manages a single SMTP session to send an email.
 * 
 * This class uses a state machine to navigate the SMTP protocol steps,
 * starting with a connection to an SMTP server (typically Gmail) and 
 * concluding with the transmission of an email message.
 */
class SmtpConnection : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs an SmtpConnection and starts the transmission process.
     * 
     * @param from The sender's email address.
     * @param to The recipient's email address.
     * @param subject The email subject line.
     * @param body The email body content.
     */
    SmtpConnection(const QString &from, const QString &to, const QString &subject, const QString &body);
    
    /**
     * @brief Destructor.
     */
    ~SmtpConnection() override;

signals:
    /**
     * @brief Signal emitted to provide status updates on the sending process.
     * @param msg A string describing the current status or result.
     */
    void status(const QString &msg);

private slots:
    /**
     * @brief Slot triggered when there is data to read from the SMTP server.
     * 
     * This function implements the core of the SMTP state machine, responding
     * to server codes and transitioning between states.
     */
    void ready_read();

    /**
     * @brief Slot to handle socket-level errors.
     * @param socketError The error type.
     */
    static void handle_error(QAbstractSocket::SocketError socketError) ;

    /**
     * @brief Slot to handle SSL-specific errors.
     * @param errors A list of SSL errors encountered.
     */
    void handle_ssl_errors(const QList<QSslError> &errors) const;

private:
    QSslSocket connection; ///< The SSL-capable socket for the SMTP connection.
    QString message;       ///< The full formatted email message (headers + body).
    QString from;          ///< The sender's email.
    QString recipient;     ///< The recipient's email.
    QString response;      ///< Buffer for the last server response.

    State state;           ///< The current state in the SMTP protocol FSM.

    /**
     * @brief Checks if the SSL socket is currently connected.
     * @return true If connected, false otherwise.
     */
    bool is_connected() const;
};

#endif //ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
