#ifndef REQUESTS_H
#define REQUESTS_H

#include <QWidget>
#include "User.h"

namespace Ui {
class Requests;
}

class Requests : public QWidget
{
    Q_OBJECT

public:
    Requests(QWidget *parent, User* _user;
    ~Requests();

private slots:
    void on_imageRequested_itemClicked(QListWidgetItem *item);

    void on_usersRequesting_itemClicked(QListWidgetItem *item);

    void on_request_clicked();

    void on_pushButton_clicked();

private:
    Ui::Requests *ui;
    User* user;
    QString send_to;
    QString image_name;
};

#endif // REQUESTS_H
