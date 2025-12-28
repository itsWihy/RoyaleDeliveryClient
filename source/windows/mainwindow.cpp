#include "../../headers/windows/mainwindow.h"

#include "../../headers/net/remotepi.h"
#include "../../headers/windows/loginwindow.h"
#include "../../headers/windows/signupwindow.h"
#include "../../headers/windows/windowutils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sign_up_button("Sign Up", this), log_in_button("Log in", this) {
    setFixedSize(500, 300);
    setToolTip("Royale Delivery Client");

    sign_up_button.setGeometry(150, 40, 200, 100);
    log_in_button.setGeometry(150, 150, 200, 100);

    connect(&sign_up_button, &QPushButton::clicked, this, &MainWindow::sign_up);
    connect(&log_in_button, &QPushButton::clicked, this, &MainWindow::log_in);
}

void MainWindow::sign_up() {
    RemotePi::get_instance().connect_to_pi();
    openAndClose(this, new SignupWindow());
}

void MainWindow::log_in() {
    RemotePi::get_instance().connect_to_pi();
    openAndClose(this, new LoginWindow());
}


MainWindow::~MainWindow() = default;
