#include "../../headers/windows/mainwindow.h"

#include "../../headers/remotepi.h"
#include "../../headers/windows/signupwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setFixedSize(500, 300);
    setToolTip("Royale Delivery Client");

    sign_up_button = new QPushButton("Sign Up", this);
    sign_up_button->setGeometry(150, 40, 200, 100);

    log_in_button = new QPushButton("Log in", this);
    log_in_button->setGeometry(150, 150, 200, 100);

    connect(sign_up_button, &QPushButton::clicked, this, &MainWindow::sign_up);
    connect(log_in_button, &QPushButton::clicked, this, &MainWindow::send_pi_hello);
}

void MainWindow::send_pi_hello() {
    RemotePi::getInstance().connect_to_pi();
}

void MainWindow::sign_up() {
    auto* signup = new SignupWindow();
    signup->show();
    this->close();
}

void MainWindow::log_in() {
}


MainWindow::~MainWindow() = default;
