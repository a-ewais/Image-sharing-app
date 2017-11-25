#include "login.h"
#include "ui_login.h"
#include<QMessageBox>
#include <QPushButton>


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
//    user = new User("10.7.57.199", 64000);
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_clicked()
{
    username= ui->username->text();
    password= ui->password->text();
    std::string _username = username.toUtf8().constData();
    std::string _password = password.toUtf8().constData();
    bool auth = user->signIn(_username, _password);
    if(auth){
        hide();
        profile = new Profile();
//        profile->fillLists();
        profile->showNormal();
    }
    else{
        QMessageBox::information(this,"ERROR","Username or Password is incorrect");
    }
}

void Login::on_signup_clicked()
{
    username= ui->username->text();
    password= ui->password->text();
    std::string _username = username.toUtf8().constData();
    std::string _password = password.toUtf8().constData();

    bool auth = user->signUp(_username, _password);
    if(auth){
        hide();
        profile = new Profile();
//        profile->fillLists();
        profile->showNormal();
    }
    else{
        QMessageBox::information(this,"ERROR","This username is already taken");
    }
}
