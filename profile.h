#ifndef PROFILE_H
#define PROFILE_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "User.h"
#include "imageinformation.h"
#include "peerprofile.h"
#include "requests.h"

namespace Ui {
class Profile;
}

class Profile : public QMainWindow
{
    Q_OBJECT

public:
    Profile(QWidget *parent, User* _user);
    ~Profile();

private slots:

    void on_upload_clicked();

    void on_request_clicked();

    void fillList();

    void on_logout_clicked();

    void on_images_itemDoubleClicked(QListWidgetItem *item);

    void on_onlineUsers_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::Profile *ui;
    User* user;
    ImageInformation* info;
    PeerProfile* peerProfile;
    Requests* request;

};

#endif // PROFILE_H
