#ifndef REQUESTS_H
#define REQUESTS_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "User.h"

namespace Ui {
class Requests;
}

class Requests : public QMainWindow
{
    Q_OBJECT

public:
    Requests(QWidget *parent, User* _user);
    ~Requests();

private slots:
    void fillList();

    void on_request_clicked();

    void on_back_clicked();

    void on_imageRequested_itemClicked(QListWidgetItem *item);

    void on_usersRequesting_itemClicked(QListWidgetItem *item);

private:
    Ui::Requests *ui;
    User* user;
    QString send_to;
    QString image_name;
    int viewsRequested;
};

#endif // REQUESTS_H
