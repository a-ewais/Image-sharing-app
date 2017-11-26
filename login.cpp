#include "login.h"
#include "ui_login.h"
#include "profile.h"
#include<QMessageBox>
#include <QPushButton>
#include <stropts.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>

int print_addresses(const int domain, vector<string>& ips)
{
int s;
struct ifconf ifconf;
struct ifreq ifr[50];
int ifs;
int i;

s = socket(domain, SOCK_STREAM, 0);
    if (s < 0) {
      perror("socket");
      return 0;
    }

    ifconf.ifc_buf = (char *) ifr;
    ifconf.ifc_len = sizeof ifr;

    if (ioctl(s, SIOCGIFCONF, &ifconf) == -1) {
      perror("ioctl");
      return 0;
    }

    ifs = ifconf.ifc_len / sizeof(ifr[0]);
    printf("interfaces = %d:\n", ifs);
    for (i = 0; i < ifs; i++) {
         char ip[INET_ADDRSTRLEN];
         struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[i].ifr_addr;
        if (!inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip))) {
            perror("inet_ntop");
            return 0;
        }
        ips.push_back(string(ip));
        printf("%s - %s\n", ifr[i].ifr_name, ip);
  }

  close(s);

  return 1;
}

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    vector<string> ips;
    print_addresses(AF_INET, ips);
    user = new User("10.7.57.199", 64000, ips[2] , 64000);
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
        ui->username->setText("");
        ui->password->setText("");
        profile = new Profile(this,user);
//        profile->fillLists();
        profile->show();
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
        ui->username->setText("");
        ui->password->setText("");
        profile = new Profile(this, user);
//        profile->fillLists();
        profile->show();
    }
    else{
        QMessageBox::information(this,"ERROR","This username is already taken");
    }
}
