#include "profile.h"
#include "ui_profile.h"
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <opencv2/core.hpp>

Profile::Profile(QWidget *parent, User* _user) :
    QMainWindow(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);
    user = _user;
    fillList();
}

Profile::~Profile()
{
    delete ui;
    delete info;
    delete peerProfile;
    delete request;
}

void Profile::fillList(){
    std::vector<std::string> images= user->viewMyImages();
    std::vector<std::string> online= user->viewOnlineUsers();


    for (int i=0; i<images.size(); ++i)
        ui->images->addItem(QString::fromStdString(images[i]));

    for (int i=0; i<online.size(); ++i){
        if(online[i]!=user->username)
            ui->onlineUsers->addItem(QString::fromStdString(online[i]));
    }
}

void Profile::on_upload_clicked()
{
    QString filename= QFileDialog::getOpenFileName(this, tr("Choose Image"), "c://", "Image file (*.jpg *.png *.jpeg);;");
    QMessageBox::information(this,tr("File Name"),filename);
    cv::Mat img = cv::imread(filename.toUtf8().constData(), cv::IMREAD_COLOR);
    user->uploadImage(filename.toUtf8().constData());
    clear();
    fillList();
//    hide();
//    request->show();
}

void Profile::on_request_clicked()
{
    request = new Requests(this,user);
    hide();
    clear();
    request->show();
}

void Profile::on_logout_clicked()
{
    user->down();
    clear();
    this->parentWidget()->show();
    this->close();
}

void Profile::on_images_itemDoubleClicked(QListWidgetItem *item)
{
    QString image_name = item->text();
    hide();
    clear();
    info = new ImageInformation(this,image_name,user);
    info->show();
}

void Profile::on_onlineUsers_itemDoubleClicked(QListWidgetItem *item)
{
    QString temp = item->text();

    peerProfile = new PeerProfile(this,temp,user);
    if(peerProfile->isEmpty())
        return;
    peerProfile->show();
    hide();
    clear();

}

void Profile::clear(){

    ui->onlineUsers->clear();
    ui->images->clear();
}

void Profile::on_refresh_clicked()
{
    clear();
    fillList();
}
