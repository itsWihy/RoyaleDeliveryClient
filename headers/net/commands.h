#ifndef ROYALEDELIVERYSERVER_COMMANDS_H
#define ROYALEDELIVERYSERVER_COMMANDS_H
#include <QCryptographicHash>
#include <QDataStream>

enum Command {
    SIGN_UP,
    LOG_IN,
    STATUS,
    ALL_MAILS,
    DELETE_A_MAIL
};

struct Email {
    QString to;
    QString from;
    QString subject;
    QString content;
}; Q_DECLARE_METATYPE(Email)

inline QDataStream &operator<<(QDataStream &out, const Email &obj) {
    out << obj.to << obj.from << obj.subject << obj.content;
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Email &obj) {
    in >> obj.to >> obj.from >> obj.subject >> obj.content;
    return in;
}


inline QByteArray pack_data(const Command cmd_type, const QStringList &parameters) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    stream << static_cast<quint32>(0) << static_cast<quint32>(cmd_type);

    for (const QString &parameter: parameters)
        stream << parameter;

    const quint32 payload_size = data.size() - sizeof(quint32);

    stream.device()->seek(0);
    stream << payload_size;

    return data;
}

inline std::string hash(const std::string &password) {
    const QByteArray data = QString(password.data()).toUtf8();
    return QString::fromUtf8(QCryptographicHash::hash(data, QCryptographicHash::Sha256)).toStdString();
}

#endif //ROYALEDELIVERYSERVER_COMMANDS_H