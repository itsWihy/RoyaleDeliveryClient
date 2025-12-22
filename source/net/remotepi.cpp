#include "../../headers/net/remotepi.h"
#include <iostream>
#include <QtWidgets>
#include <QtNetwork>


RemotePi::RemotePi() : connection(this) {
    connect(&connection, &QAbstractSocket::errorOccurred, this, &RemotePi::print_error);
    connect(&connection, &QTcpSocket::readyRead, this, &RemotePi::read_from_pi);
}

bool RemotePi::is_connected() const {
    return connection.state() == QTcpSocket::ConnectedState;
}

qint32 byte_array_to_int(QByteArray source) {
    qint32 size;
    QDataStream temp{&source, QIODevice::WriteOnly};
    temp >> size;
    return size;
}

QByteArray int_to_byte_array(qint32 number) {
    QByteArray size;
    QDataStream temp{&size, QIODevice::WriteOnly};
    temp << number;
    return size;
}

bool RemotePi::write_to_pi_raw(QByteArray &data) {
    if (!is_connected()) return false;

    connection.write(int_to_byte_array(data.size()));
    connection.write(data);

    return connection.waitForBytesWritten();
}

QString RemotePi::read_from_pi() {
    QString data = "";

    QByteArray buffer {};
    qint32 size = 0;

    while (connection.bytesAvailable() > 0) {
        buffer.append(connection.readAll());

        while ((size == 0 && buffer.size() >= 4) || (size > 0 && buffer.size() >= size)) {
            if (size == 0 && buffer.size() >= 4) {
                size = byte_array_to_int( buffer.mid(0, 4));
                buffer.remove(0, 4);
            }

            if (size > 0 && buffer.size() >= size) {
                data = QString::fromUtf8(buffer.mid(0, size));
            }
        }
    }

    return data;
}

void RemotePi::connect_to_pi() {
    std::cout << "Connecting to PI..." << std::endl;
    connection.abort();
    connection.connectToHost(QHostAddress("192.168.1.156"), 5004);

    if (is_connected())
        std::cout << "Connected successfully!" << std::endl;
 }

bool RemotePi::sign_up(const QString &name, const QString &password) {
    if (!is_connected()) return false;

    QByteArray toSend{};

    toSend.append(int_to_byte_array(name.size()));
    toSend.append(name.toUtf8());

    toSend.append(int_to_byte_array(password.size()));
    toSend.append(password.toUtf8());

    return write_to_pi_raw(toSend);
}

bool RemotePi::log_in(QString name, QString password) {
    return false;
}

void RemotePi::print_error(const QAbstractSocket::SocketError socketError) {
    std::cout << "Error: " << socketError;
}
