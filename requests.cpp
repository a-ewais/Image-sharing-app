#include "requests.h"
#include "ui_requests.h"

Requests::Requests(QWidget *parent,User* _user) :
    QMainWindow(parent),
    ui(new Ui::Requests)
{
    ui->setupUi(this);
    user = _user;
    send_to="";
    image_name="";
//    requestedViews = _x;
    fillList();
}

Requests::~Requests()
{
    delete ui;
}

void Requests::clear(){
    ui->imageRequested->clear();
    ui->usersRequesting->clear();
}

void Requests::fillList(){
   std::vector<std::string> u= user->getRequestedImages();
   for(int i=0; i<u.size(); ++i)
      ui->imageRequested->addItem(QString::fromStdString(u[i]));
}

void Requests::on_request_clicked()
{
    if (send_to!="" && image_name!=""){
        QString _s= ui->views->text();
        std::string s = _s.toUtf8().constData();
        user->grantPeerImage(send_to.toUtf8().constData(), image_name.toUtf8().constData(), std::stoi(s));
    }
}

void Requests::on_back_clicked()
{
    clear();
    this->parentWidget()->show();
    this->close();
}

void Requests::on_imageRequested_itemClicked(QListWidgetItem *item)
{
    ui->usersRequesting->clear();
    image_name = item->text();
        std::vector<std::string> u= user->getRequesters(image_name.toUtf8().constData());
        for(int i=0; i<u.size(); ++i)
           ui->usersRequesting->addItem(QString::fromStdString(u[i]));
}

void Requests::on_usersRequesting_itemClicked(QListWidgetItem *item)
{
    send_to = item->text();
}

void Requests::on_refresh_clicked()
{
    clear();
    fillList();
}
