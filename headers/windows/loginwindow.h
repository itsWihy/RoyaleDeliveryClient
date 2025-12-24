//
// Created by Wihy on 12/24/25.
//

#ifndef ROYALEDELIVERYCLIENT_LOGINWINDOW_H
#define ROYALEDELIVERYCLIENT_LOGINWINDOW_H
#include <qlineedit.h>
#include <QMainWindow>
#include <QPushButton>

#include "../net/commands.h"


class LoginWindow : public QMainWindow {
    Q_OBJECT

private:
    QLineEdit name_button;
    QLineEdit password_button;
    QPushButton log_in_button;
    QPushButton go_back_button;

private slots:
    bool log_in() const;

    void go_back();
    void handle_server_cmd(Command cmd, QString message) const;

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow() override;
};


#endif //ROYALEDELIVERYCLIENT_LOGINWINDOW_H