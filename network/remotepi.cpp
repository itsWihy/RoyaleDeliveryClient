#include "remotepi.h"
#include <iostream>
#include <QtWidgets>
#include <QtNetwork>

RemotePi::RemotePi(QWidget *parent) : connection(new QTcpSocket(this)) {
    connect(connection, &QAbstractSocket::errorOccurred, this, &RemotePi::print_error);
}

void RemotePi::connect_to_pi() {
    std::cout << " yo";
    connection->abort();
    connection->connectToHost(QHostAddress("192.168.1.290"), 8080, QIODeviceBase::ReadWrite);
}

void RemotePi::print_error(QAbstractSocket::SocketError socketError) {
        std::cout << "Error: " << socketError;
}

RemotePi::~RemotePi() {
    delete connection;
}
