#include "requests.h"
#include "ui_requests.h"

Requests::Requests(QWidget *parent,User* _user) :
    QWidget(parent),
    ui(new Ui::Requests)
{
    ui->setupUi(this);
    user = _user;
    send_to="";
    image_name="";
    fillList();
}

Requests::~Requests()
{
    delete ui;
}

void Requests::fillList(){
   std::vector<std::string> u= user->requesters();
   for(int i=0; i<u.size(); ++i)
      ui->imageRequested->addItem(QString::fromStdString(u[i]));
}

void Requests::on_imageRequested_itemClicked(QListWidgetItem *item)
{
    image_name = item->text();
}

void Requests::on_usersRequesting_itemClicked(QListWidgetItem *item)
{
    send_to = item->text();
    std::vector<std::string> u= user->requestedImages(send_to.toUtf8().constData());
    for(int i=0; i<u.size(); ++i)
       ui->imageRequested->addItem(QString::fromStdString(u[i]));
}

void Requests::on_request_clicked()
{
    if (send_to!="" && image_name!=""){
        QString _s= ui->view_count->text();
        std::string s = _s.toUtf8().constData();
        int x= std::stoi(s);
        user->grantPeerImage(send_to.toUtf8().constData(), image_name.toUtf8().constData(), x);
    }
}

void Requests::on_back_clicked()
{

}