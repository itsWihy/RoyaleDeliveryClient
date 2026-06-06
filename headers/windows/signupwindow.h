/**
 * @file signupwindow.h
 * @brief Header for the sign-up window UI.
 */

#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <qdialogbuttonbox.h>
#include <qlineedit.h>
#include <QMainWindow>
#include <QPushButton>
#include <QObject>

#include "../net/commands.h"

/**
 * @class SignupWindow
 * @brief Window that allows new users to register an account.
 */
class SignupWindow : public QMainWindow {
    Q_OBJECT

private:
    QLineEdit name_button;      ///< Input field for the desired username.
    QLineEdit password_button;  ///< Input field for the desired password (masked).
    QPushButton sign_up_button; ///< Button to trigger the registration process.
    QPushButton go_back_button; ///< Button to return to the main entry window.

private slots:
    /**
     * @brief Initiates the registration process by sending new credentials to the server.
     * @return true if the sign-up command was successfully sent.
     */
    bool sign_up() const;

    /**
     * @brief Closes this window and returns to the previous one.
     */
    void go_back();

    /**
     * @brief Processes responses from the server regarding the sign-up attempt.
     * @param cmd The command type received.
     * @param message The server's response message (e.g., "Account created").
     */
    void handle_server_cmd(Command cmd, QString message);

public:
    /**
     * @brief Constructs the sign-up window.
     * @param parent The parent widget.
     */
    explicit SignupWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~SignupWindow() override;

};

#endif // SIGNUPWINDOW_H
