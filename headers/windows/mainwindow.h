#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QPushButton sign_up_button;
    QPushButton log_in_button;

private slots:
    void sign_up();
    void log_in();

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
};

#endif // MAINWINDOW_H
