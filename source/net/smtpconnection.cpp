//
// Created by Wihy on 12/28/25.
//

#include "../../headers/net/smtpconnection.h"

SmtpConnection::SmtpConnection(const QString &from, const QString &to, const QString &subject, const QString &body)
    : connection(new QTcpSocket(this)) {

    connect(connection, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(connection, SIGNAL(connected()), this, SLOT(connected()));
    connect(connection, SIGNAL(disconnectedFromHost()), this, SLOT(disconnected()));

    message = "To: " + to + "\n";
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append(body);
    message.replace(QString::fromLatin1("\n"),  QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    this->from = from;
    recipient = to;
    state = State::INIT;
    connection->connectToHost("smtp.yourserver.com", 25);

    if (connection->waitForConnected(30000)) { qDebug("connected"); }

    text_stream = new QTextStream(connection);
}

SmtpConnection::~SmtpConnection() {
    delete text_stream;
    delete connection;
}

void SmtpConnection::disconnected() {
    qDebug() << "disconneted";
    qDebug() << "error " << connection->errorString();
}

void SmtpConnection::connected() {
    qDebug() << "SMTP Connected";
}

void SmtpConnection::readyRead() { //todo: lmfao shit code do fix. write SERVER equivalent
    qDebug() << "readyRead";
    // SMTP is line-oriented

    QString responseLine;
    do {
        responseLine = connection->readLine();
        response += responseLine;
    } while (connection->canReadLine() && responseLine[3] != ' ');

    responseLine.truncate(3);

    switch (state);

    if (state == State::INIT && responseLine[0] == '2') {
        // banner was okay, let's go on

        *text_stream << "HELO there\r\n";
        text_stream->flush();

        state = State::MAIL;
    } else if (state == State::MAIL && responseLine[0] == '2') {
        // HELO response was okay (well, it has to be)
        *text_stream << "MAIL FROM: " << from << "\r\n";
        text_stream->flush();
        state = State::RCPT;
    } else if (state == State::RCPT && responseLine[0] == '2') {
        *text_stream << "RCPT TO: " << recipient << "\r\n"; //r
        text_stream->flush();
        state = State::DATA;
    } else if (state == State::DATA && responseLine[0] == '2') {
        *text_stream << "DATA\r\n";
        text_stream->flush();
        state = State::BODY;
    } else if (state == State::BODY && responseLine[0] == '3') {
        *text_stream << message << "\r\n.\r\n";
        text_stream->flush();
        state = State::QUIT;
    } else if (state == State::QUIT && responseLine[0] == '2') {
        *text_stream << "QUIT\r\n";
        text_stream->flush();
        // here, we just close.
        state = State::CLOSE;
        emit status(tr("Message sent"));
    } else if (state == State::CLOSE) {
        deleteLater();
        return;
    } else {
        // something broke.
        QMessageBox::warning(nullptr, tr("Qt Mail Example"), tr("Unexpected reply from SMTP server:\n\n") + response);
        state = State::CLOSE;
    }
    response = "";
}
