#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(500, 300);
    setToolTip("Royale Delivery Client");

    sign_up_button = new QPushButton("Sign Up", this);
    sign_up_button->setGeometry(150, 40, 200, 100);

    log_in_button = new QPushButton("Log in", this);
    log_in_button->setGeometry(150, 150, 200, 100);

}

MainWindow::~MainWindow() {}
