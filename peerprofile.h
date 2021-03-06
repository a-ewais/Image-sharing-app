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
    bool isEmpty();

private slots:

    void on_request_clicked();

    void fillList();

    void on_back_clicked();

    void on_imageList_itemClicked(QListWidgetItem *item);

    void on_refresh_clicked();

    void clear();

private:
    Ui::PeerProfile *ui;
    User* user;
    QString online_user;
    QString imagename;
    vector<string> tempList;

};

#endif // PEERPROFILE_H
