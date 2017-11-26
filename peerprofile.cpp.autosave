#include "peerprofile.h"
#include "ui_peerprofile.h"
#include <opencv2/core.hpp>

PeerProfile::PeerProfile(QWidget *parent, QString str,User* _user) :
    QMainWindow(parent),
    ui(new Ui::PeerProfile)
{
    ui->setupUi(this);
    user = _user;
    online_user = str;
    imagename = "";
    fillList();
}

PeerProfile::~PeerProfile()
{
    delete ui;
}

void PeerProfile::fillList(){
    std::vector<std::string> temp;
    temp= user->getListOfPeerImages(online_user.toUtf8().constData());
    for (int i=0; i<temp.size(); ++i)
        ui->imageList->addItem(QString::fromStdString(temp[i]));
}

void PeerProfile::on_request_clicked()
{
    std::string _imagename = imagename.toUtf8().constData();
    std::string _online = online_user.toUtf8().constData();
    QString x = ui->count->text();
    std:: string _num = x.toUtf8().constData();

    if(imagename!=""&& x!="")
        user->requestPeerImage(_online, _imagename, stoi(_num));
}

void PeerProfile::on_back_clicked()
{
    this->parentWidget()->show();
    this->close();
}

void PeerProfile::on_imageList_itemClicked(QListWidgetItem *item)
{
    imagename = item->text();
//    ui->views->setText(QString::number());
    std::string _name = imagename.toUtf8().constData();
    int x = user->viewsCount(online_user.toUtf8().constData(), _name);
    cv::Mat img;
    if (x!=0){
        img = user->viewPeerImage(online_user.toUtf8().constData(), _name);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        ui->image->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
    }
}
