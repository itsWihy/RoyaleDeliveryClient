/**
 * @file mainwindow.h
 * @brief Header for the initial entry window of the application.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

/**
 * @class MainWindow
 * @brief The first window shown to the user, providing options to sign up or log in.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QPushButton sign_up_button; ///< Button to navigate to the Sign Up window.
    QPushButton log_in_button;  ///< Button to navigate to the Log In window.

private slots:
    /**
     * @brief Slot to handle clicking the Sign Up button.
     * Opens the SignupWindow and closes this one.
     */
    void sign_up();

    /**
     * @brief Slot to handle clicking the Log In button.
     * Opens the LoginWindow and closes this one.
     */
    void log_in();

public:
    /**
     * @brief Constructs the main window.
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainWindow() override;
};

#endif // MAINWINDOW_H
