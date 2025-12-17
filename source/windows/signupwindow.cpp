#include "../../headers/windows/signupwindow.h"

#include <iostream>
#include <qdebug.h>
#include <qlabel.h>
#include <qstatusbar.h>

#include "../../headers/windows/mainwindow.h"

SignupWindow::SignupWindow(QWidget *parent) : QMainWindow(parent),
                                              name_button(new QLineEdit(this)),
                                              password_button(new QLineEdit(this)),
                                              sign_up_button(new QPushButton("Sign Up", this)),
                                              go_back_button(new QPushButton("Go Back", this)) {
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(500, 300);
    setToolTip("Royale Sign UP!");

    name_button->setGeometry(180, 50, 200, 30);
    password_button->setGeometry(180, 100, 200, 30);

    const auto nameLabel = new QLabel("Enter name", this);
    nameLabel->setGeometry(80, 50, 90, 30);

    const auto passwordLabel = new QLabel("Enter password", this);
    passwordLabel->setGeometry(80, 100, 90, 30);

    go_back_button->setGeometry(120, 180, 120, 50);
    sign_up_button->setGeometry(260, 180, 120, 50);

    connect(sign_up_button, &QPushButton::clicked, this, &SignupWindow::sign_up);
    connect(go_back_button, &QPushButton::clicked, this, &SignupWindow::go_back);
}

void SignupWindow::sign_up() const {
    const QString name = name_button->text();
    const QString password = password_button->text();

    if (name.isEmpty() || password.isEmpty()) {
        this->statusBar()->setStyleSheet("color: red");
        this->statusBar()->showMessage("Please fill both fields", 5000);
        return;
    }

    qDebug().nospace() << "Name " << name << " and " << password;
    //send Pi a packet.
}

void SignupWindow::go_back() {
    auto* window = new MainWindow();
    window->show();
    this->close();
}

SignupWindow::~SignupWindow() = default;
