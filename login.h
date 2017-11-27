#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "profile.h"
#include "User.h"
#include "ServiceDiscovery.h"
#include <QString>

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_login_clicked(); //checks if the username exists with a correct entered password it grants the user the access to profile screen.
                             //if the username or password is incorrect a notification will be displayed.

    void on_signup_clicked(); //checks if the username does not exist it grants the user the access to profile screen otherwise asks for a new user

    void loadLogo();
private:
    Ui::Login *ui;
    QString username;
    QString password;
    User* user;
    Profile *profile;
    ServiceDiscovery* sd;
};

#endif // LOGIN_H
