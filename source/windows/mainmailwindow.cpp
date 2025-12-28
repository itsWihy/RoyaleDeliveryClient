//
// Created by Wihy on 12/28/25.
//

#include "../../headers/windows/mainmailwindow.h"

#include <qboxlayout.h>
#include <qlistwidget.h>

MainMailWindow::MainMailWindow(QWidget *parent)
    : QMainWindow(parent), central(this), compose_button("Compose", this), mailList(this), separator(this) {
    setFixedSize(1000, 600);
    setToolTip("Royale Delivery Client");
    setCentralWidget(&central);

    QVBoxLayout *mainLayout = new QVBoxLayout(&central);

    mailList.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mailList.setSpacing(8);
    mailList.setStyleSheet(
        "QListWidget::item {"
        "  border-bottom: 1px solid #2c2c2c;"
        "  padding: 6px;"
        "}"
    );

    separator.setFrameShape(QFrame::HLine);
    separator.setFrameShadow(QFrame::Sunken);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(&compose_button);
    bottomLayout->addStretch();

    mainLayout->addWidget(&mailList);
    mainLayout->addWidget(&separator);
    mainLayout->addLayout(bottomLayout);
}

//TODO: CMD to retrieve emails from server, add to list here.

MainMailWindow::~MainMailWindow() = default;
