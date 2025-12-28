#include "../headers/windows/mainwindow.h"

#include <QApplication>

#include "../headers/windows/signupwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto* window = new MainWindow();
    window->show();

    return QApplication::exec();
}
