#ifndef SERVICEGUI_H
#define SERVICEGUI_H

#include <QMainWindow>
#include "ServiceDiscovery.h"

namespace Ui {
class ServiceGUI;
}

class ServiceGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServiceGUI(QWidget *parent = 0);
    ~ServiceGUI();

private:
    Ui::ServiceGUI *ui;
    ServiceDiscovery* serv;
};

#endif // SERVICEGUI_H
