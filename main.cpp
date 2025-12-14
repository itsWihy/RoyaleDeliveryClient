#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include "network/remotepi.h"

static RemotePi pi{};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow window;
    window.show();
//TODO: button handling in main window. add correct local PI ip and write server PI code.
    pi.connect_to_pi();

    return a.exec();
}
