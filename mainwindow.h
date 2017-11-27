#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "servicegui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


    void on_service_clicked();

    void on_peer_clicked();

private:
    Ui::MainWindow *ui;
    Login* log;
    ServiceGUI* serv;
};

#endif // MAINWINDOW_H
