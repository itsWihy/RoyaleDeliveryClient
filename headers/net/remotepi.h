/**
 * @file remotepi.h
 * @brief Singleton class for managing the main socket connection to the server.
 */

#ifndef REMOTEPI_H
#define REMOTEPI_H

#include <QTcpSocket>
#include <QDataStream>

#include "commands.h"

/**
 * @class RemotePi
 * @brief Handles low-level network communication with the RoyaleDeliveryServer.
 * 
 * This class follows the Singleton pattern to provide a centralized point for all 
 * client-server interactions.
 */
class RemotePi : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Gets the singleton instance of RemotePi.
     * @return RemotePi& Reference to the static instance.
     */
    static RemotePi& get_instance() {
        static RemotePi instance;
        return instance;
    }

    // Disable copy constructor and assignment operator to enforce Singleton
    RemotePi(const RemotePi&) = delete;
    void operator=(const RemotePi&) = delete;

private:
    QTcpSocket connection; ///< The actual TCP socket connection to the server.
    QDataStream input;     ///< Data stream for reading incoming server data.
    QDataStream output;    ///< Data stream for writing outgoing server data.

    /**
     * @brief Private constructor for Singleton.
     */
    RemotePi();

    /**
     * @brief Checks if the socket is currently connected to the server.
     * @return true If connected, false otherwise.
     */
    bool is_connected() const;

    /**
     * @brief Formats and sends a command with parameters to the server.
     * 
     * @param cmd_type The Command enum to send.
     * @param parameters The list of string arguments for the command.
     * @return true If the data was successfully written to the socket.
     */
    bool send_cmd_to_server(Command cmd_type, const QStringList& parameters);

    /**
     * @brief Internal slot to read and process incoming data from the server.
     */
    void handle_server_data() const;

private slots:
    /**
     * @brief Slot to handle socket errors.
     * @param socketError The error type.
     */
    static void handle_error(QAbstractSocket::SocketError socketError) ;

public:
    /**
     * @brief Establishes a connection to the server using PI_ADDRESS.
     */
    void connect_to_pi();

    /**
     * @brief Sends a sign-up request to the server.
     * @param name The desired username.
     * @param password The plain-text password.
     * @return true if the command was sent.
     */
    bool sign_up(const QString &name, const QString &password);

    /**
     * @brief Sends a log-in request to the server.
     * @param name The username.
     * @param password The plain-text password.
     * @return true if the command was sent.
     */
    bool log_in(const QString& name, const QString& password);

    /**
     * @brief Sends a request to delete a specific email.
     * @param hash The unique hash of the email to delete.
     * @return true if the command was sent.
     */
    bool delete_mail(const QString& hash);

    /**
     * @brief Sends a request to fetch all emails for the current user.
     * @return true if the command was sent.
     */
    bool fetch_emails();

signals:
    /**
     * @brief Signal emitted when a general status message is received from the server.
     * @param cmd The command associated with the message.
     * @param message The status/error message string.
     */
    void server_message_received(Command cmd, QString message) const;

    /**
     * @brief Signal emitted when the server sends the list of all emails.
     * @param mails A QVector of Email objects.
     */
    void received_all_mails(QVector<Email> mails) const;
};

#endif // REMOTEPI_H
