//
// Created by Wihy on 12/24/25.
//

/**
 * @file loginwindow.h
 * @brief Header for the login window UI.
 */

#ifndef ROYALEDELIVERYCLIENT_LOGINWINDOW_H
#define ROYALEDELIVERYCLIENT_LOGINWINDOW_H
#include <qlineedit.h>
#include <QMainWindow>
#include <QPushButton>

#include "../net/commands.h"

/**
 * @class LoginWindow
 * @brief Window that allows existing users to log into their account.
 */
class LoginWindow : public QMainWindow {
    Q_OBJECT

private:
    QLineEdit name_button;     ///< Input field for the username.
    QLineEdit password_button; ///< Input field for the password (masked).
    QPushButton log_in_button;  ///< Button to trigger the login process.
    QPushButton go_back_button; ///< Button to return to the main entry window.

private slots:
    /**
     * @brief Initiates the login process by sending credentials to the server.
     * @return true if the login command was successfully sent.
     */
    bool log_in() const;

    /**
     * @brief Closes this window and returns to the previous one.
     */
    void go_back();

    /**
     * @brief Processes responses from the server regarding the login attempt.
     * @param cmd The command type received.
     * @param message The server's response message (e.g., "Logged in successfully").
     */
    void handle_server_cmd(Command cmd, QString message);

public:
    /**
     * @brief Constructs the login window.
     * @param parent The parent widget.
     */
    explicit LoginWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~LoginWindow() override;
};


#endif //ROYALEDELIVERYCLIENT_LOGINWINDOW_H