#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QListWidgetItem>
#include "User.h"
#include "imageinformation.h"
#include "peerprofile.h"
#include "requests.h"

namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    Profile(QWidget *parent, User* _user);
    ~Profile();

private slots:
    void on_images_itemClicked(QListWidgetItem *item);

    void on_onlineUsers_itemClicked(QListWidgetItem *item);

    void on_upload_clicked();

    void on_request_clicked();

    void fillList();

private:
    Ui::Profile *ui;
    User* user;
    ImageInformation* info;
    PeerProfile* peerProfile;
    Requests* request;

};

#endif // PROFILE_H
