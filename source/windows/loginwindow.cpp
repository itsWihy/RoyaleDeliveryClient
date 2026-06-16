//
// Created by Wihy on 12/24/25.
//

#include "../../headers/windows/loginwindow.h"

#include <qlabel.h>
#include <qstatusbar.h>

#include "../../headers/globals.h"
#include "../../headers/net/remotepi.h"
#include "../../headers/windows/mainmailwindow.h"
#include "../../headers/windows/mainwindow.h"
#include "../../headers/windows/windowutils.h"

/**
 * @brief Constructs the LoginWindow.
 * 
 * Initializes UI components, sets up layouts manually, and connects signals 
 * for buttons and server communication.
 * 
 * @param parent Parent widget.
 */
LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent),
                                            name_button(this),
                                            password_button(this),
                                            log_in_button("Log In", this),
                                            go_back_button("Go Back", this) {
    // Ensure object is cleaned up when closed
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(500, 300);
    setToolTip("Royale LOGGG innn!");

    // Manual geometry placement for UI elements
    name_button.setGeometry(180, 50, 200, 30);
    password_button.setGeometry(180, 100, 200, 30);
    // Mask password input for security
    password_button.setEchoMode(QLineEdit::Password);

    const auto name_label = new QLabel("Enter name", this);
    name_label->setGeometry(80, 50, 90, 30);

    const auto password_label = new QLabel("Enter password", this);
    password_label->setGeometry(80, 100, 90, 30);

    go_back_button.setGeometry(120, 180, 120, 50);
    log_in_button.setGeometry(260, 180, 120, 50);

    // Connect button clicks to their respective slots
    connect(&log_in_button, &QPushButton::clicked, this, &LoginWindow::log_in);
    connect(&go_back_button, &QPushButton::clicked, this, &LoginWindow::go_back);

    // Listen for server responses globally via RemotePi singleton
    connect(&RemotePi::get_instance(), SIGNAL(server_message_received(Command,QString)), this, SLOT(handle_server_cmd(Command,QString)));
}

/**
 * @brief Validates input and sends login request to the server.
 * 
 * Performs basic client-side validation (length checks) before attempting 
 * network communication.
 * 
 * @return true if the login command was sent successfully.
 */
bool LoginWindow::log_in() const {
    const QString name = name_button.text();
    const QString password = password_button.text();

    // Enforce name and password length constraints
    if (name.isEmpty() || password.isEmpty() || name.length() > 8 || password.length() > 8)
        return error_popup(this, "Name and password must be between 1 and 8 characters long.");

    // Attempt to send login command via singleton
    if (const bool success = RemotePi::get_instance().log_in(name, password); !success)
        return error_popup(this, "Couldn't reach server");

    // Temporarily store credentials in globals for session use
    CLIENT_NAME = name;
    RAW_PASSWORD = password;
    return true;
}

/**
 * @brief Handles incoming server commands relevant to login.
 * 
 * Specifically looks for messages starting with "LOGIN" to determine if 
 * authentication was successful.
 * 
 * @param cmd Command type.
 * @param message Server response string.
 */
void LoginWindow::handle_server_cmd(const Command cmd, QString message) {
    if (!message.startsWith("LOGIN")) return;

    if (cmd == STATUS) {
        // Strip the "LOGIN" prefix added by RemotePi::handle_server_data
        message.remove(0,5);

        if (message == "TRUE") {
            // Success: Transition to the Main Mail Window
            auto* window = new MainMailWindow();
            success_popup(window, "Logging you in...");
            openAndClose(this, window);

        } else {
            // Failure: Show error popup
            error_popup(this, "Something went wrong.. check connection and try a different username/password ");
        }
    }
}

/**
 * @brief Returns to the main entry window.
 */
void LoginWindow::go_back() {
    openAndClose(this, new MainWindow());
}

/**
 * @brief Default destructor.
 */
LoginWindow::~LoginWindow() = default;