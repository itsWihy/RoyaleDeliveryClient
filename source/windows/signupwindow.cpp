#include "../../headers/windows/signupwindow.h"

#include <iostream>
#include <qdebug.h>
#include <qlabel.h>
#include <QNetworkAccessManager>
#include <qstatusbar.h>

#include "../../headers/windows/mainwindow.h"
#include "../../headers/net/remotepi.h"
#include "../../headers/windows/loginwindow.h"
#include "../../headers/windows/windowutils.h"

SignupWindow::SignupWindow(QWidget *parent) : QMainWindow(parent),
                                              name_button(this),
                                              password_button(this),
                                              sign_up_button("Sign Up", this),
                                              go_back_button("Go Back", this) {
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
    connect(&RemotePi::get_instance(), SIGNAL(server_message_received(Command,QString)), this, SLOT(handle_server_cmd(Command,QString)));
}

bool SignupWindow::sign_up() const {
    const QString name = name_button.text();
    const QString password = password_button.text();

    if (name.isEmpty() || password.isEmpty() || name.length() > 8 || password.length() > 8)
        return error_popup(this, "Name and password must be between 1 and 8 characters long.");

    if (name.contains(":") || password.contains(":"))
        return error_popup(this, "Name and password may not contain a colon");

    if (const bool success = RemotePi::get_instance().sign_up(name, password); !success)
        return error_popup(this, "Couldn't reach server");

    return true;
}

void SignupWindow::handle_server_cmd(const Command cmd, QString message) {
    if (!message.startsWith("SIGNUP")) return;

    if (cmd == STATUS) {
        message.remove(0,6);

        if (message == "TRUE") {
            auto* login = new LoginWindow();
            openAndClose(this, login);

            success_popup(login, "Successfully registered!");
        } else {
            error_popup(this, "Something went wrong.. check connection and try a different username");
        }
    }
}


void SignupWindow::go_back() {
    openAndClose(this, new MainWindow());
}

SignupWindow::~SignupWindow() = default;


