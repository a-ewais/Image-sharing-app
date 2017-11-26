#ifndef PEERPROFILE_H
#define PEERPROFILE_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "User.h"

namespace Ui {
class PeerProfile;
}

class PeerProfile : public QMainWindow
{
    Q_OBJECT

public:
    PeerProfile(QWidget *parent, QString str, User* _user);
    ~PeerProfile();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_request_clicked();

    void fillList();

    void on_back_clicked();

private:
    Ui::PeerProfile *ui;
    User* user;
    QString online_user;
    QString imagename;
};

#endif // PEERPROFILE_H
