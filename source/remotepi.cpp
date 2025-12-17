#include "../headers/remotepi.h"
#include <iostream>
#include <QtWidgets>
#include <QtNetwork>

RemotePi::RemotePi() : connection(new QTcpSocket(this)) {
    connect(connection, &QAbstractSocket::errorOccurred, this, &RemotePi::print_error);
}

void RemotePi::connect_to_pi() const {
    std::cout << "Connecting to PI..." << std::endl;
    connection->abort();
    connection->connectToHost(QHostAddress("192.168.1.41"), 8080);
}

bool RemotePi::sign_up(QString name, QString password) {
    //send to PI: Name, Password


    return false;
}

bool RemotePi::log_in(QString name, QString password) {
    return false;
}

void RemotePi::print_error(const QAbstractSocket::SocketError socketError) {
    std::cout << "Error: " << socketError;
}
