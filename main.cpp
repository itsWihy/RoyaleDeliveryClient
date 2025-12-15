#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include "network/remotepi.h"
#include "global.h"

RemotePi PI{};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow window;
    window.show();

    return a.exec();
}
