#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include "network/remotepi.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow window;
    window.show();
//https://wiki.qt.io/Qt_for_Beginners The observer pattern IS USEFUL READ!
//https://wiki.qt.io/How_to_Use_QPushButton IS USEFUL READ!
    return a.exec();
}
