/**
 * @file main.cpp
 * @brief Entry point for the RoyaleDeliveryClient application.
 */

#include "../headers/windows/mainwindow.h"

#include <QApplication>

#include "../headers/windows/signupwindow.h"

/**
 * @brief Main function.
 * 
 * Initializes the Qt application, creates the main entry window, 
 * and starts the event loop.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit code.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create and show the main entry window
    auto* window = new MainWindow();
    window->show();

    // Execute the application event loop
    return QApplication::exec();
}
