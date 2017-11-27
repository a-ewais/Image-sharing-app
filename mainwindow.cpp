#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_service_clicked()
{
    serv = new ServiceGUI(this);
    hide();
    serv->show();
}

void MainWindow::on_peer_clicked()
{
    log = new Login(this);
    hide();
    log->show();
}
