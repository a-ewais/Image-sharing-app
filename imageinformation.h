#ifndef IMAGEINFORMATION_H
#define IMAGEINFORMATION_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "User.h"

namespace Ui {
class ImageInformation;
}

class ImageInformation : public QMainWindow
{
    Q_OBJECT

public:
    ImageInformation(QWidget *parent, QString str, User* _user);
    ~ImageInformation();

private slots:
    void on_changeView_clicked();


    void on_delete_2_clicked();

    void on_viewersList_itemClicked(QListWidgetItem *item);

    void on_back_clicked();

private:
    Ui::ImageInformation *ui;
    QString imagename;
    User* user;
    QString peer;
    void fillList();
    void displayImage();
};

#endif // IMAGEINFORMATION_H
