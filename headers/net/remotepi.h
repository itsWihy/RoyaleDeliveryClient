#ifndef REMOTEPI_H
#define REMOTEPI_H

#include <QTcpSocket>
#include <QDataStream>

#include "commands.h"

class RemotePi : public QObject {
    Q_OBJECT

public:
    static RemotePi& get_instance() {
        static RemotePi instance;
        return instance;
    }

    RemotePi(const RemotePi&) = delete;
    void operator=(const RemotePi&) = delete;

private:
    QTcpSocket connection;
    QDataStream input;
    QDataStream output;

    RemotePi();

    bool is_connected() const;
    bool send_cmd_to_pi(Command cmd_type, const QStringList& parameters);
    QString read_from_pi();

private slots:
    static void handle_error(QAbstractSocket::SocketError socketError) ;

public:
    void connect_to_pi();

    bool sign_up(const QString &name, const QString &password);
    bool log_in(const QString& name, const QString& password);
};

#endif // REMOTEPI_H
