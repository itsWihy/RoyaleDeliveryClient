#ifndef REMOTEPI_H
#define REMOTEPI_H

#include <QTcpSocket>

class RemotePi : public QObject {
    Q_OBJECT

private:
    QTcpSocket* connection = nullptr;

private slots:
    void print_error(QAbstractSocket::SocketError socketError);

public:
    RemotePi(QWidget *parent = nullptr);
    ~RemotePi();

    void connect_to_pi() const;

    bool sign_up(QString name, QString password);
    bool log_in(QString name, QString password);
};

#endif // REMOTEPI_H
