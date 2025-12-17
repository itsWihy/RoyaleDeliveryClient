#include "../../headers/windows/signupwindow.h"

#include <qlabel.h>
#include <QPushButton>

SignupWindow::SignupWindow(QWidget *parent) : QMainWindow(parent),
                                              name_button(new QLineEdit(this)),
                                              password_button(new QLineEdit(this)),
                                              sign_up_button(new QPushButton("Sign Up", this)),
                                              go_back_button(new QPushButton("Go Back", this)) {
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(500, 300);
    setToolTip("Royale Sign UP!");

    name_button->setGeometry(180, 50, 200, 30);
    password_button->setGeometry(180, 100, 200, 30);

    const auto nameLabel = new QLabel(tr("Enter name"), this);
    nameLabel->setGeometry(80, 50, 90, 30);
    nameLabel->setBuddy(name_button);

    const auto passwordLabel = new QLabel(tr("Enter password"), this);
    passwordLabel->setGeometry(80, 100, 90, 30);
    passwordLabel->setBuddy(name_button);

    go_back_button->setGeometry(120, 180, 120, 50);
    sign_up_button->setGeometry(260, 180, 120, 50);
}

SignupWindow::~SignupWindow() = default;
