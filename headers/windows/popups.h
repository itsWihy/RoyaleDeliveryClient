//
// Created by Wihy on 12/24/25.
//

#ifndef ROYALEDELIVERYCLIENT_POPUPS_H
#define ROYALEDELIVERYCLIENT_POPUPS_H
#include <QMainWindow>
#include <qstatusbar.h>

inline bool error_popup(const QMainWindow* const window, const QString &error) {
    window->statusBar()->setStyleSheet("color: red");
    window->statusBar()->showMessage(error, 5000);
    return false;
}

inline bool success_popup(const QMainWindow* const window, const QString &success) {
    window->statusBar()->setStyleSheet("color: green");
    window->statusBar()->showMessage(success, 5000);
    return true;
}

#endif //ROYALEDELIVERYCLIENT_POPUPS_H