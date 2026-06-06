/**
 * @file mainwindow.cpp
 * @brief Implementation of the main entry window.
 */

#include "../../headers/windows/mainwindow.h"

#include "../../headers/net/remotepi.h"
#include "../../headers/windows/loginwindow.h"
#include "../../headers/windows/signupwindow.h"
#include "../../headers/windows/windowutils.h"

/**
 * @brief Constructs the MainWindow.
 * 
 * Sets up the initial choice for the user: Sign Up or Log In.
 * 
 * @param parent Parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sign_up_button("Sign Up", this), log_in_button("Log in", this) {
    setFixedSize(500, 300);
    setToolTip("Royale Delivery Client");

    // Position the two main action buttons
    sign_up_button.setGeometry(150, 40, 200, 100);
    log_in_button.setGeometry(150, 150, 200, 100);

    // Connect button clicks to their transition slots
    connect(&sign_up_button, &QPushButton::clicked, this, &MainWindow::sign_up);
    connect(&log_in_button, &QPushButton::clicked, this, &MainWindow::log_in);
}

/**
 * @brief Transitions to the Sign Up window.
 * 
 * Ensures a connection to the server is initiated before switching windows.
 */
void MainWindow::sign_up() {
    RemotePi::get_instance().connect_to_pi();
    openAndClose(this, new SignupWindow());
}

/**
 * @brief Transitions to the Log In window.
 * 
 * Ensures a connection to the server is initiated before switching windows.
 */
void MainWindow::log_in() {
    RemotePi::get_instance().connect_to_pi();
    openAndClose(this, new LoginWindow());
}

/**
 * @brief Default destructor.
 */
MainWindow::~MainWindow() = default;
