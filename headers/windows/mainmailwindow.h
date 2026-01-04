//
// Created by Wihy on 12/28/25.
//

#ifndef ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H
#define ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H
#include <qlistwidget.h>
#include <QMainWindow>
#include <qobjectdefs.h>
#include <QPushButton>
#include <QStackedWidget>
#include <qtextedit.h>


class QLabel;
struct Email;

class MainMailWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget central;
    QPushButton compose_button;
    QListWidget mailList;
    QFrame separator;

    QStackedWidget *stackedWidget;
    QWidget *inboxPage;
    QWidget *composePage;

    QLineEdit *recipientEdit;
    QLineEdit *subjectEdit;
    QTextEdit *bodyEdit;

    QWidget *viewPage;
    QLabel *viewSubjectLabel;
    QLabel *viewFromLabel;
    QTextEdit *viewBodyDisplay;
    QVector<Email> currentEmails;

public:
    explicit MainMailWindow(QWidget* parent = nullptr);
    ~MainMailWindow() override = default;

private slots:
    void display_mails_from_server(const QVector<Email>& emails);
    void delete_mail_from_server(const Email &email) const;

public slots:
    void compose() const;
};


#endif //ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H