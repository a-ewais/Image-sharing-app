#include "peerprofile.h"
#include "ui_peerprofile.h"
#include <opencv2/core.hpp>
#include <sys/stat.h>
#include <dirent.h>

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
    temp = user->getListOfPeerImages(online_user.toUtf8().constData());
    tempList = temp;
    if(isEmpty()){
        string username = online_user.toUtf8().constData();
        string path = "./LoadedImages/" + username;
        DIR* dir_point = opendir(path.c_str());
        dirent* entry = readdir(dir_point);
        while (entry){									// if !entry then end of directory
            if (entry->d_type == DT_REG){		// if entry is a regular file
                std::string fname = entry->d_name;	// filename
                if(fname != ".default.jpg")
                    temp.push_back(fname.substr(5));		// add filename to results vector
            }
            entry = readdir(dir_point);
        }

    }
    tempList = temp;

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

bool PeerProfile::isEmpty(){
    return tempList.size() == 0;
}

void PeerProfile::on_back_clicked()
{
    clear();
    this->parentWidget()->show();
    this->close();
}

void PeerProfile::clear(){

    ui->imageList->clear();
    ui->image->clear();
}
void PeerProfile::on_imageList_itemClicked(QListWidgetItem *item)
{
    ui->image->clear();
    imagename = item->text();
    std::string _name = imagename.toUtf8().constData();
    int x = user->allowedViews(online_user.toUtf8().constData(), _name);
    ui->views->setText(QString::number(x));
    cv::Mat img;
    if (x!=0){
        img = user->viewPeerImage(online_user.toUtf8().constData(), _name);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        cv::Size size(640,610);
        cv::resize(img,img,size);
        ui->image->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
    }
}

void PeerProfile::on_refresh_clicked()
{
    clear();
    fillList();
}
