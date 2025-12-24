//
// Created by Wihy on 12/24/25.
//

#include "../../headers/windows/loginwindow.h"

#include <qlabel.h>
#include <qstatusbar.h>

#include "../../headers/net/remotepi.h"
#include "../../headers/windows/mainwindow.h"
#include "../../headers/windows/popups.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent),
                                            name_button(this),
                                            password_button(this),
                                            log_in_button("Log In", this),
                                            go_back_button("Go Back", this) {
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(500, 300);
    setToolTip("Royale LOGGG innn!");

    name_button.setGeometry(180, 50, 200, 30);
    password_button.setGeometry(180, 100, 200, 30);

    const auto name_label = new QLabel("Enter name", this);
    name_label->setGeometry(80, 50, 90, 30);

    const auto password_label = new QLabel("Enter password", this);
    password_label->setGeometry(80, 100, 90, 30);

    go_back_button.setGeometry(120, 180, 120, 50);
    log_in_button.setGeometry(260, 180, 120, 50);

    connect(&log_in_button, &QPushButton::clicked, this, &LoginWindow::log_in);
    connect(&go_back_button, &QPushButton::clicked, this, &LoginWindow::go_back);
    connect(&RemotePi::get_instance(), SIGNAL(server_message_received(Command,QString)), this, SLOT(handle_server_cmd(Command,QString)));
}

bool LoginWindow::log_in() const {
    const QString name = name_button.text();
    const QString password = password_button.text();

    if (name.isEmpty() || password.isEmpty() || name.length() > 8 || password.length() > 8)
        return error_popup(this, "Name and password must be between 1 and 8 characters long.");

    if (const bool success = RemotePi::get_instance().log_in(name, password); !success)
        return error_popup(this, "Couldn't reach server");

    return true;
}


void LoginWindow::handle_server_cmd(const Command cmd, QString message) const {
    if (!message.startsWith("LOGIN")) return;

    if (cmd == STATUS) {
        message.remove(0,5);

        if (message == "TRUE") {
            success_popup(this, "Successfully logined!!!!");
        } else {
            error_popup(this, "Something went wrong.. check connection and try a different username/password ");
        }
    }
}


void LoginWindow::go_back() {
    auto* window = new MainWindow();
    window->show();
    this->close();
}

LoginWindow::~LoginWindow() = default;