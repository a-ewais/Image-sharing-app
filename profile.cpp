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
    request = new Requests(this,user);
}

Profile::~Profile()
{
    delete ui;
}

void Profile::fillList(){
    std::vector<std::string> images= user->viewMyImages();
    std::vector<std::string> online= user->viewOnlineUsers();


    for (int i=0; i<images.size(); ++i)
        ui->images->addItem(QString::fromStdString(images[i]));

    for (int i=0; i<online.size(); ++i)
        ui->onlineUsers->addItem(QString::fromStdString(online[i]));
}

void Profile::on_images_itemClicked(QListWidgetItem *item)
{
    QString image_name = item->text();
    hide();
    info = new ImageInformation(this,image_name,user);
    info->showNormal();
}

void Profile::on_onlineUsers_itemClicked(QListWidgetItem *item)
{
    QString temp = item->text();
    hide();
    peerProfile = new PeerProfile(this,temp,user);
    peerProfile->showNormal();

}

void Profile::on_upload_clicked()
{
    QString filename= QFileDialog::getOpenFileName(this, tr("Choose Image"), "c://", "Image file (*.*);;");
    QMessageBox::information(this,tr("File Name"),filename);
    cv::Mat img = cv::imread(filename.toUtf8().constData(), cv::IMREAD_COLOR);
    user->uploadImage(filename.toUtf8().constData());
    hide();
    request->showNormal();
}

void Profile::on_request_clicked()
{
    hide();
    request->showNormal();
}

void Profile::on_logout_clicked()
{
    this->parentWidget()->show();
    this->close();
}
