//
// Created by Wihy on 12/28/25.
//

#include "../../headers/windows/mainmailwindow.h"

#include <iostream>
#include <qboxlayout.h>
#include <qlistwidget.h>

#include "../../headers/net/remotepi.h"
#include "../../headers/net/smtpconnection.h"

#include <QLabel>
#include <qlineedit.h>
#include <QStackedWidget>
#include <qtextedit.h>

#include "../../headers/globals.h"

/**
 * @brief Constructs the MainMailWindow.
 * 
 * Sets up the complex multi-page layout using QStackedWidget. Pages include:
 * 0: Inbox (list of emails)
 * 1: Compose (writing a new email)
 * 2: View (reading a selected email)
 * 
 * @param parent Parent widget.
 */
MainMailWindow::MainMailWindow(QWidget *parent)
    : QMainWindow(parent), central(this), compose_button("Compose", this), mailList(this), separator(this) {
    setFixedSize(1000, 600);
    setToolTip("Royale Delivery Client");
    setCentralWidget(&central);

    auto *rootLayout = new QVBoxLayout(&central);

    // Welcome label at the top
    auto welcomeLabel = new QLabel("Welcome, " + CLIENT_NAME, this);
    welcomeLabel->setStyleSheet(
        "font-size: 18px; "
        "font-weight: bold; "
        "color: #00000f; "
        "padding: 10px;"
    );
    welcomeLabel->setAlignment(Qt::AlignCenter);

    rootLayout->addWidget(welcomeLabel);

    stackedWidget = new QStackedWidget(this);
    rootLayout->addWidget(stackedWidget);

    // --- Page 0: Inbox ---
    inboxPage = new QWidget();
    auto *inboxLayout = new QVBoxLayout(inboxPage);

    mailList.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mailList.setSpacing(8);
    mailList.setStyleSheet(
        "QListWidget::item {"
        "  border-bottom: 1px solid #2c2c2c;"
        "  padding: 6px;"
        "}"
    );

    separator.setFrameShape(QFrame::HLine);
    separator.setFrameShadow(QFrame::Sunken);

    auto *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(&compose_button);
    bottomLayout->addStretch();

    inboxLayout->addWidget(&mailList);
    inboxLayout->addWidget(&separator);
    inboxLayout->addLayout(bottomLayout);

    // --- Page 1: Compose ---
    composePage = new QWidget();
    const auto compose_layout = new QVBoxLayout(composePage);

    recipientEdit = new QLineEdit();
    recipientEdit->setPlaceholderText("To (Recipient Email)");

    subjectEdit = new QLineEdit();
    subjectEdit->setPlaceholderText("Subject");

    bodyEdit = new QTextEdit();
    bodyEdit->setPlaceholderText("Type your message here...");

    const auto compose_buttons = new QHBoxLayout();
    auto *send_button = new QPushButton("Send Mail");
    auto *backBtn = new QPushButton("Back to Inbox");

    compose_buttons->addStretch();
    compose_buttons->addWidget(backBtn);
    compose_buttons->addWidget(send_button);

    compose_layout->addWidget(new QLabel("<h3>Compose New Message</h3>"));
    compose_layout->addWidget(recipientEdit);
    compose_layout->addWidget(subjectEdit);
    compose_layout->addWidget(bodyEdit);
    compose_layout->addLayout(compose_buttons);

    // --- Page 2: View Email ---
    viewPage = new QWidget();
    auto *viewLayout = new QVBoxLayout(viewPage);

    viewSubjectLabel = new QLabel();
    viewSubjectLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    viewFromLabel = new QLabel();
    viewBodyDisplay = new QTextEdit();
    viewBodyDisplay->setReadOnly(true);

    auto *viewButtons = new QHBoxLayout();
    auto *viewBackBtn = new QPushButton("Back to Inbox");
    auto *deleteBtn = new QPushButton("Delete Mail");
    deleteBtn->setStyleSheet("background-color: #442222; color: white;");

    viewButtons->addStretch();
    viewButtons->addWidget(deleteBtn);
    viewButtons->addWidget(viewBackBtn);

    viewLayout->addWidget(viewSubjectLabel);
    viewLayout->addWidget(viewFromLabel);
    viewLayout->addWidget(viewBodyDisplay);
    viewLayout->addLayout(viewButtons);

    stackedWidget->addWidget(inboxPage);
    stackedWidget->addWidget(composePage);
    stackedWidget->addWidget(viewPage);

    // Navigation and Logic Connections
    auto refreshAndGoHome = [this] {
        mailList.clearSelection();
        RemotePi::get_instance().fetch_emails();
        stackedWidget->setCurrentIndex(0);
    };

    connect(&compose_button, &QPushButton::clicked, this, [this] { stackedWidget->setCurrentIndex(1); });
    connect(backBtn, &QPushButton::clicked, this, refreshAndGoHome);
    connect(viewBackBtn, &QPushButton::clicked, this, refreshAndGoHome);

    connect(send_button, &QPushButton::clicked, this, &MainMailWindow::compose);

    // Handle mail deletion
    connect(deleteBtn, &QPushButton::clicked, this, [this] {
        if (const int row = mailList.currentRow(); row >= 0 && row < currentEmails.size()) {
            this->delete_mail_from_server(currentEmails[row]);
        }
    });

    // Handle selecting a mail from the list to view it
    connect(&mailList, &QListWidget::currentRowChanged, this, [this](const int row) {
        if (row >= 0 && row < currentEmails.size()) {
            const auto &email = currentEmails[row];
            viewSubjectLabel->setText("Subject: " + email.subject);
            viewFromLabel->setText("From: " + email.from);
            viewBodyDisplay->setPlainText(email.content);
            stackedWidget->setCurrentIndex(2);
        }
    });

    // Initial fetch of emails from the server
    RemotePi::get_instance().fetch_emails();
    // Connect to server signal to update list when data arrives
    connect(&RemotePi::get_instance(), &RemotePi::received_all_mails, this, &MainMailWindow::display_mails_from_server);
}

/**
 * @brief Collects data from the Compose page and sends an email via SmtpConnection.
 * 
 * Creates a new SmtpConnection (which handles the SMTP protocol asynchronously).
 * Clears the fields and returns to the Inbox upon initiation.
 */
void MainMailWindow::compose() const {
    const QString to = recipientEdit->text();
    const QString subject = subjectEdit->text();
    const QString body = bodyEdit->toPlainText();

    if (to.isEmpty() || subject.isEmpty() || body.isEmpty()) {
        QMessageBox::warning(const_cast<MainMailWindow*>(this), tr("Validation Error"), tr("Please fill in all fields before sending."));
        return;
    }

    // Locate the send button to disable it during sending
    for (auto *btn : findChildren<QPushButton*>()) {
        if (btn->text() == "Send Mail") {
            btn->setEnabled(false);
            break;
        }
    }

    // Initiate SMTP transmission
    SmtpConnection *connection = new SmtpConnection{CLIENT_NAME + "@royalemail.com", to, subject, body};

    // When SMTP finishes, refresh the inbox to show the sent mail and show feedback
    // Use Qt::QueuedConnection to prevent nested event loops (from QMessageBox) 
    // from processing events that might delete the SmtpConnection while its readyRead is on the stack.
    connect(connection, &SmtpConnection::status, this, [this](const QString &message) {
        // Re-enable send button
        for (auto *btn : findChildren<QPushButton*>()) {
            if (btn->text() == "Send Mail") {
                btn->setEnabled(true);
                break;
            }
        }

        if (message == tr("Message sent")) {
            QMessageBox::information(const_cast<MainMailWindow*>(this), tr("Success"), tr("Email sent successfully!"));
            RemotePi::get_instance().fetch_emails();
            stackedWidget->setCurrentIndex(0);
            
            // Reset UI only on success
            recipientEdit->clear();
            subjectEdit->clear();
            bodyEdit->clear();
        } else {
            QMessageBox::critical(const_cast<MainMailWindow*>(this), tr("Error"), message);
        }
    }, Qt::QueuedConnection);
}

/**
 * @brief Populates the QListWidget with Email objects received from the server.
 * 
 * Differentiates between [SENT] and [RECEIVED] mails based on the sender's address.
 * 
 * @param emails List of Email objects to display.
 */
void MainMailWindow::display_mails_from_server(const QVector<Email> &emails) {
    mailList.blockSignals(true); // Prevent triggering currentRowChanged while clearing

    mailList.clear();
    currentEmails = emails;

    const QString myEmail = CLIENT_NAME + "@royalemail.com";

    for (const Email &email: emails) {
        QString statusPrefix = (email.from == myEmail) ? "[SENT] " : "[RECEIVED] ";
        QString routingInfo = QString("From: %1 → To: %2").arg(email.from, email.to);

        QString displayText = QString("%1 %2\n%3")
                .arg(statusPrefix)
                .arg(email.subject)
                .arg(routingInfo);

        mailList.addItem(displayText);
    }


    mailList.blockSignals(false);
    mailList.setCurrentRow(-1); // Reset selection
    mailList.clearSelection();
}

/**
 * @brief Requests the server to delete a specific email.
 * 
 * Computes the hash of the email content and sends a DELETE_A_MAIL command.
 * 
 * @param email The Email object to delete.
 */
void MainMailWindow::delete_mail_from_server(const Email &email) {
    const auto hashed_value = hash(email.content.toStdString());
    RemotePi::get_instance().delete_mail(QString::fromStdString(hashed_value));

    std::cout << "Deleting mail of hash: " << hashed_value << std::endl;;

    // Optimistically clear the list and reset view before refresh
    mailList.blockSignals(true);
    mailList.clear();
    currentEmails.clear();
    mailList.setCurrentRow(-1);
    mailList.blockSignals(false);

    stackedWidget->setCurrentIndex(0);
    // Refresh list from server to reflect deletion
    RemotePi::get_instance().fetch_emails();
}
