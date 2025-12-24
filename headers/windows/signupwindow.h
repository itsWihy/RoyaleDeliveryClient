#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <qdialogbuttonbox.h>
#include <qlineedit.h>
#include <QMainWindow>
#include <QPushButton>
#include <QObject>

#include "../net/commands.h"

class SignupWindow : public QMainWindow {
    Q_OBJECT

private:
    QLineEdit name_button;
    QLineEdit password_button;
    QPushButton sign_up_button;
    QPushButton go_back_button;

private slots:
    void go_back();
    void sign_up() const;
    void handle_server_cmd(Command cmd, QString message) const;

public:
    explicit SignupWindow(QWidget* parent = nullptr);
    ~SignupWindow() override;

};

#endif // SIGNUPWINDOW_H
