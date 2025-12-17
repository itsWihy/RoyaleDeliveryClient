#ifndef REMOTEPI_H
#define REMOTEPI_H

#include <QTcpSocket>

class RemotePi : public QObject {
    Q_OBJECT

public:
    static RemotePi& getInstance() {
        static RemotePi instance;
        return instance;
    }

    RemotePi(const RemotePi&) = delete;
    void operator=(const RemotePi&) = delete;

private:
    QTcpSocket* connection = nullptr;

    RemotePi();

private slots:
    void print_error(QAbstractSocket::SocketError socketError);

public:
    void connect_to_pi() const;

    bool sign_up(QString name, QString password);
    bool log_in(QString name, QString password);
};

#endif // REMOTEPI_H
