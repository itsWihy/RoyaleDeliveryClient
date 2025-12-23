#include "../../headers/windows/signupwindow.h"

#include <iostream>
#include <qdebug.h>
#include <qlabel.h>
#include <QNetworkAccessManager>
#include <qstatusbar.h>

#include "../../headers/windows/mainwindow.h"
#include "../../headers/net/remotepi.h"

SignupWindow::SignupWindow(QWidget *parent) : QMainWindow(parent),
                                              name_button(this),
                                              password_button(this),
                                              sign_up_button("Sign Up", this),
                                              go_back_button("Go Back", this) {

    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(500, 300);
    setToolTip("Royale Sign UP!");

    name_button.setGeometry(180, 50, 200, 30);
    password_button.setGeometry(180, 100, 200, 30);

    const auto name_label = new QLabel("Enter name", this);
    name_label->setGeometry(80, 50, 90, 30);

    const auto password_label = new QLabel("Enter password", this);
    password_label->setGeometry(80, 100, 90, 30);

    go_back_button.setGeometry(120, 180, 120, 50);
    sign_up_button.setGeometry(260, 180, 120, 50);

    connect(&sign_up_button, &QPushButton::clicked, this, &SignupWindow::sign_up);
    connect(&go_back_button, &QPushButton::clicked, this, &SignupWindow::go_back);
}

void SignupWindow::sign_up() const {
    const QString name = name_button.text();
    const QString password = password_button.text();

    if (name.isEmpty() || password.isEmpty()) {
        this->statusBar()->setStyleSheet("color: red");
        this->statusBar()->showMessage("Please fill both fields", 5000);
        return;
    }

    bool success = RemotePi::get_instance().sign_up(name, password);
    qDebug().nospace() << "Name " << name << " and " << password << " Suc: " << success;
}

void SignupWindow::go_back() {
    auto* window = new MainWindow();
    window->show();
    this->close();
}

SignupWindow::~SignupWindow() = default;
