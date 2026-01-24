//
// Created by Wihy on 12/28/25.
//

#ifndef ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
#define ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
#include <QObject>
#include <qobjectdefs.h>

#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

enum class State {
    INIT,
    TLS_REQ,
    TLS_START,
    AUTH_REQ,
    AUTH_USER,
    AUTH_PASS,
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
    void ready_read();
    static void handle_error(QAbstractSocket::SocketError socketError) ;

    void handle_ssl_errors(const QList<QSslError> &errors) const;

private:
    QSslSocket connection;
    QString message;
    QString from;
    QString recipient;
    QString response;

    State state;

    bool is_connected() const;
};

#endif //ROYALEDELIVERYCLIENT_SMTPCONNECTION_H
