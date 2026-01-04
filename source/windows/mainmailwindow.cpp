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

MainMailWindow::MainMailWindow(QWidget *parent)
    : QMainWindow(parent), central(this), compose_button("Compose", this), mailList(this), separator(this) {
    setFixedSize(1000, 600);
    setToolTip("Royale Delivery Client");
    setCentralWidget(&central);

    auto *rootLayout = new QVBoxLayout(&central);

    stackedWidget = new QStackedWidget(this);
    rootLayout->addWidget(stackedWidget);


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

    auto refreshAndGoHome = [this] {
        mailList.clearSelection();
        RemotePi::get_instance().fetch_emails();
        stackedWidget->setCurrentIndex(0);
    };

    connect(&compose_button, &QPushButton::clicked, this, [this] { stackedWidget->setCurrentIndex(1); });
    connect(backBtn, &QPushButton::clicked, this, refreshAndGoHome);
    connect(viewBackBtn, &QPushButton::clicked, this, refreshAndGoHome);

    connect(send_button, &QPushButton::clicked, this, &MainMailWindow::compose);

    connect(deleteBtn, &QPushButton::clicked, this, [this] {
        if (const int row = mailList.currentRow(); row >= 0 && row < currentEmails.size()) {
            this->delete_mail_from_server(currentEmails[row]);
        }
    });

    connect(&mailList, &QListWidget::currentRowChanged, this, [this](const int row) {
        if (row >= 0 && row < currentEmails.size()) {
            const auto &email = currentEmails[row];
            viewSubjectLabel->setText("Subject: " + email.subject);
            viewFromLabel->setText("From: " + email.from);
            viewBodyDisplay->setPlainText(email.content);
            stackedWidget->setCurrentIndex(2);
        }
    });

    RemotePi::get_instance().fetch_emails();
    connect(&RemotePi::get_instance(), &RemotePi::received_all_mails, this, &MainMailWindow::display_mails_from_server);
}

void MainMailWindow::compose() const {
    const QString to = recipientEdit->text();
    const QString subject = subjectEdit->text();
    const QString body = bodyEdit->toPlainText();

    new SmtpConnection{CLIENT_NAME + "@royalemail.com", to, subject, body};

    recipientEdit->clear();
    subjectEdit->clear();
    bodyEdit->clear();

    stackedWidget->setCurrentIndex(0);
    RemotePi::get_instance().fetch_emails();
}

void MainMailWindow::display_mails_from_server(const QVector<Email> &emails) {
    mailList.clear();
    currentEmails = emails;

    for (const auto &email: emails) {
        mailList.addItem(QString(email.subject + "\n" + email.from));
    }
}

void MainMailWindow::delete_mail_from_server(const Email &email) const {
    const std::string full_email = "To: " + email.to.toStdString() + "\r\n" +
                             "From: " + email.from.toStdString() + "\r\n" +
                             "Subject: " + email.subject.toStdString() + "\r\n" +
                             email.content.toStdString() + "\r\n";
//To: gas\r\nFrom: f@royalemail.com\r\nSubject: gsg\r\ngasg\r\n
    std::cout << full_email;

    const auto hashed_value = hash(full_email);
    RemotePi::get_instance().delete_mail(QString::fromStdString(hashed_value));

    std::cout << "Deleting mail of hash: " << hashed_value  << std::endl;;

    RemotePi::get_instance().fetch_emails();
    stackedWidget->setCurrentIndex(0);
}
