#include "imageinformation.h"
#include "ui_imageinformation.h"
#include <opencv2/core.hpp>

ImageInformation::ImageInformation(QWidget *parent, QString str, User* _user) :
    QMainWindow(parent),
    ui(new Ui::ImageInformation)
{
    ui->setupUi(this);
    imagename = str;
    user = _user;
    peer="";

    ui->views->setText("");
    ui->imageName->setText(imagename);
    displayImage();
    fillList();
}

void ImageInformation::displayImage(){
    cv::Mat img;
    std::string _name=imagename.toUtf8().constData();
    img = user->viewMyImage(_name);
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    cv::Size size(720,640);
    cv::resize(img,img,size);
    ui->image->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}

void ImageInformation::fillList(){
    std::vector<std::string> temp;
    std::string _name = imagename.toUtf8().constData();
    temp = user->whoCanView(_name);

    for(int i=0; i<temp.size(); ++i)
        ui->viewersList->addItem(QString::fromStdString(temp[i]));
}

ImageInformation::~ImageInformation()
{
    delete ui;
}

void ImageInformation::on_changeView_clicked()
{
    QString s= ui->views->text();
    std::string _s = s.toUtf8().constData();
    int views = std::stoi(_s);
    if (peer!="" && s!="" )
        user->updatePeerImage(peer.toUtf8().constData(),imagename.toUtf8().constData(), views);
}


void ImageInformation::on_delete_2_clicked()
{
    if (peer!="")
        user->revokePeerImage(peer.toUtf8().constData(), imagename.toUtf8().constData());
}

void ImageInformation::on_back_clicked()
{
    this->parentWidget()->show();
    this->close();
}

void ImageInformation::on_viewersList_itemClicked(QListWidgetItem *item)
{
    peer = item->text();
    std::string s= peer.toUtf8().constData();
    std::string _name=imagename.toUtf8().constData();
    ui->views->setText(QString::number(user->viewsCount(s,_name)));
}
