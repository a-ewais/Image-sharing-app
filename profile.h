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

    void on_upload_clicked(); // uploading of images type jpg from the disk
    void on_request_clicked(); // moves to the requests screen to see who requested the images
    void fillList(); // fills the online list and images the user owns
    void on_logout_clicked(); // logout and returns to the login screen
    void on_images_itemDoubleClicked(QListWidgetItem *item); // if image is chosen it goes to imageinformation screen to display the information related to this image
    void on_onlineUsers_itemDoubleClicked(QListWidgetItem *item); // goes to the peerprofile of the online user
    void on_refresh_clicked();
    void clear();

private:
    Ui::Profile *ui;
    User* user;
    ImageInformation* info;
    PeerProfile* peerProfile;
    Requests* request;

};

#endif // PROFILE_H
