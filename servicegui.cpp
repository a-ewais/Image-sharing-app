#include "servicegui.h"
#include "ui_servicegui.h"

ServiceGUI::ServiceGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServiceGUI)
{
    ui->setupUi(this);
    serv = new ServiceDiscovery("localhost",64000);
}

ServiceGUI::~ServiceGUI()
{
    delete ui;
    delete serv;
}
