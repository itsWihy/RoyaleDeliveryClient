//
// Created by Wihy on 12/28/25.
//

/**
 * @file mainmailwindow.h
 * @brief Header for the main email application window.
 */

#ifndef ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H
#define ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H
#include <qlistwidget.h>
#include <QMainWindow>
#include <qobjectdefs.h>
#include <QPushButton>
#include <QStackedWidget>
#include <qtextedit.h>
#include <QVector>

#include "../net/commands.h"


class QLabel;

/**
 * @class MainMailWindow
 * @brief The primary interface for viewing, composing, and managing emails.
 * 
 * This window uses a QStackedWidget to switch between the Inbox, Compose, 
 * and View Email pages.
 */
class MainMailWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget central;            ///< The central widget container.
    QPushButton compose_button;  ///< Button to open the compose page.
    QListWidget mailList;       ///< List widget displaying the user's emails.
    QFrame separator;           ///< Visual separator.

    QStackedWidget *stackedWidget; ///< Widget for switching between different views.
    QWidget *inboxPage;            ///< The page displaying the list of emails.
    QWidget *composePage;          ///< The page for writing a new email.

    // Compose Page Widgets
    QLineEdit *recipientEdit;      ///< Input field for the recipient's address.
    QLineEdit *subjectEdit;        ///< Input field for the email subject.
    QTextEdit *bodyEdit;           ///< Text area for the email body.

    // View Page Widgets
    QWidget *viewPage;             ///< The page for viewing a selected email's content.
    QLabel *viewSubjectLabel;      ///< Label to display the subject in View mode.
    QLabel *viewFromLabel;         ///< Label to display the sender in View mode.
    QTextEdit *viewBodyDisplay;    ///< Text area to display the email body in View mode.

    QVector<Email> currentEmails;  ///< Cached list of emails retrieved from the server.

public:
    /**
     * @brief Constructs the main mail window.
     * @param parent The parent widget.
     */
    explicit MainMailWindow(QWidget* parent = nullptr);

    /**
     * @brief Default destructor.
     */
    ~MainMailWindow() override = default;

private slots:
    /**
     * @brief Updates the UI with a new list of emails from the server.
     * @param emails The list of Email objects to display.
     */
    void display_mails_from_server(const QVector<Email>& emails);

    /**
     * @brief Sends a request to the server to delete a specific email.
     * @param email The Email object to be deleted.
     */
    void delete_mail_from_server(const Email &email);

public slots:
    /**
     * @brief Switches the UI to the Compose page.
     */
    void compose() const;
};


#endif //ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H