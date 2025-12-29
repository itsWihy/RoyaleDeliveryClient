//
// Created by Wihy on 12/28/25.
//

#ifndef ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
#define ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
#include <QObject>
#include <qobjectdefs.h>

#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

enum class State {
    INIT,
    MAIL,
    RCPT,
    DATA,
    BODY,
    QUIT,
    CLOSE
};

class SmtpConnection : public QObject {
    Q_OBJECT

public:
    SmtpConnection(const QString &from, const QString &to, const QString &subject, const QString &body);
    ~SmtpConnection() override;

signals:
    void status(const QString &msg);

private slots:
    void disconnected();
    void connected();
    void readyRead();

private:
    QTextStream *text_stream;
    QTcpSocket *connection;
    QString message;
    QString from;
    QString recipient;
    QString response;

    State state;
};

#endif //ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
