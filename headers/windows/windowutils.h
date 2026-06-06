//
// Created by Wihy on 12/24/25.
//

/**
 * @file windowutils.h
 * @brief Utility functions for UI operations, popups, and window management.
 */

#ifndef ROYALEDELIVERYCLIENT_POPUPS_H
#define ROYALEDELIVERYCLIENT_POPUPS_H
#include <QMainWindow>
#include <qstatusbar.h>

/**
 * @brief Displays a red error message in the window's status bar for 5 seconds.
 * 
 * @param window The window where the status bar is located.
 * @param error The error message string to display.
 * @return false always (utility for combining with return statements).
 */
inline bool error_popup(const QMainWindow* const window, const QString &error) {
    window->statusBar()->setStyleSheet("color: red");
    window->statusBar()->showMessage(error, 5000);
    return false;
}

/**
 * @brief Displays a green success message in the window's status bar for 5 seconds.
 * 
 * @param window The window where the status bar is located.
 * @param success The success message string to display.
 * @return true always (utility for combining with return statements).
 */
inline bool success_popup(const QMainWindow* const window, const QString &success) {
    window->statusBar()->setStyleSheet("color: green");
    window->statusBar()->showMessage(success, 5000);
    return true;
}

/**
 * @brief Transitions from one window to another.
 * Shows the 'open' window, closes the 'close' window, and schedules 'close' for deletion.
 * 
 * @param close The window to be closed.
 * @param open The window to be shown.
 */
inline void openAndClose(QMainWindow* close, QMainWindow* open) {
    open->show();
    close->close();
    close->deleteLater();
}

#endif //ROYALEDELIVERYCLIENT_POPUPS_H