//
// Created by Wihy on 12/28/25.
//

#ifndef ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H
#define ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H
#include <qlistwidget.h>
#include <QMainWindow>
#include <qobjectdefs.h>
#include <QPushButton>


class MainMailWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget central;
    QPushButton compose_button;
    QListWidget mailList;
    QFrame separator;

public:
    explicit MainMailWindow(QWidget* parent = nullptr);
    ~MainMailWindow() override;
};


#endif //ROYALEDELIVERYCLIENT_MAINMAILWINDOW_H