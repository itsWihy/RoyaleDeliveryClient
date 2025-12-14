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

    void connect_to_pi();
};

#endif // REMOTEPI_H
