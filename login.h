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
    void on_login_clicked();

    void on_signup_clicked();

private:
    Ui::Login *ui;
    QString username;
    QString password;
    User* user;
    Profile *profile;
    ServiceDiscovery* sd;
};

#endif // LOGIN_H
