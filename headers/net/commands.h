/**
 * @file commands.h
 * @brief Defines the communication protocol, including commands and data structures.
 */

#ifndef ROYALEDELIVERYSERVER_COMMANDS_H
#define ROYALEDELIVERYSERVER_COMMANDS_H
#include <QCryptographicHash>
#include <QDataStream>
#include <QIODevice>

/**
 * @brief Enum representing the different commands that can be sent to or received from the server.
 */
enum Command {
    SIGN_UP,        ///< Request to create a new user account.
    LOG_IN,         ///< Request to authenticate an existing user.
    STATUS,         ///< General status response from the server (success/error messages).
    ALL_MAILS,      ///< Request to retrieve all emails for the logged-in user.
    DELETE_A_MAIL   ///< Request to delete a specific email.
};

/**
 * @brief Structure representing an Email message.
 */
struct Email {
    QString to;      ///< Recipient's address.
    QString from;    ///< Sender's address.
    QString subject; ///< Subject line of the email.
    QString content; ///< Body content of the email.
}; Q_DECLARE_METATYPE(Email)

/**
 * @brief Serializes an Email object into a QDataStream.
 * 
 * @param out The data stream to write to.
 * @param obj The Email object to serialize.
 * @return QDataStream& Reference to the output stream.
 */
inline QDataStream &operator<<(QDataStream &out, const Email &obj) {
    out << obj.to << obj.from << obj.subject << obj.content;
    return out;
}

/**
 * @brief Deserializes an Email object from a QDataStream.
 * 
 * @param in The data stream to read from.
 * @param obj The Email object to populate.
 * @return QDataStream& Reference to the input stream.
 */
inline QDataStream &operator>>(QDataStream &in, Email &obj) {
    in >> obj.to >> obj.from >> obj.subject >> obj.content;
    return in;
}

/**
 * @brief Packs command and parameters into a QByteArray for network transmission.
 * 
 * The packet structure is: [payload_size (4 bytes)] [command_type (4 bytes)] [parameters (variable length strings)]
 * 
 * @param cmd_type The Command to be packed.
 * @param parameters A list of strings representing the command's arguments.
 * @return QByteArray The formatted data packet ready to be sent over a socket.
 */
inline QByteArray pack_data(const Command cmd_type, const QStringList &parameters) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    // Placeholder for payload size, will be overwritten later
    stream << static_cast<quint32>(0) << static_cast<quint32>(cmd_type);

    for (const QString &parameter: parameters)
        stream << parameter;

    const quint32 payload_size = data.size() - sizeof(quint32);

    // Go back to the beginning to write the actual payload size
    stream.device()->seek(0);
    stream << payload_size;

    return data;
}

/**
 * @brief Computes the SHA-256 hash of a string (typically a password).
 * 
 * @param password The plain-text password.
 * @return std::string The hex-encoded hash string.
 */
inline std::string hash(const std::string &password) {
    const QByteArray data = QString(password.data()).toUtf8();
    return QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex().toStdString();
}

#endif //ROYALEDELIVERYSERVER_COMMANDS_H