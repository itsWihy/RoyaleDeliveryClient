#ifndef REMOTEPI_H
#define REMOTEPI_H

#include <QTcpSocket>

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
    QTcpSocket* connection = nullptr;
    QDataStream* input;
    QDataStream* output;

    RemotePi();

    bool is_connected() const;
    bool write_to_pi_raw(const QByteArray& data) const;
    QString read_from_pi();

private slots:
    void print_error(QAbstractSocket::SocketError socketError);

public:
    void connect_to_pi() const;

    bool sign_up(const QString &name, const QString &password) const;
    bool log_in(QString name, QString password);
};

#endif // REMOTEPI_H
