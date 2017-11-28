#include "ServerPeer.h"
#include "Parameter.h"
#include "MessageDecoder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <map>

using namespace std;

ServerPeer::ServerPeer(char* _listen_hostname, int _listen_port, Client* _serviceDiscoveryClient):Server(_listen_hostname, _listen_port){
	mkdir("MyImages", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("LoadedImages", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	myPath = get_current_dir_name();
    myImagesPath = "./MyImages/";
    loadedImagesPath = "./LoadedImages/";
	serviceDiscoveryClient = _serviceDiscoveryClient;

	pthread_t p;
	pthread_create( &p, NULL, &Server::startListen, this);
}

ServerPeer::~ServerPeer(){

}

vector<string> ServerPeer::loadFileNames(string path){
	cout << "Wslt" << endl;
	std::vector<std::string> results;
	DIR* dir_point = opendir(path.c_str());
	dirent* entry = readdir(dir_point);
	while (entry){									// if !entry then end of directory
		if (entry->d_type == DT_REG){		// if entry is a regular file
			std::string fname = entry->d_name;	// filename
            if(fname != ".default.jpeg")
				results.push_back(fname.substr(5));		// add filename to results vector
		}
		entry = readdir(dir_point);
	}

	return results;
}

std::vector<std::string> ServerPeer::getListofImages(std::string username, std::string token){
	cout << "SERVERPEER::getListofImages!";
	std::vector<std::string> results;

	if(serviceDiscoveryClient->auth(username, token))
		results = loadFileNames(myImagesPath);

	return results;
}

std::string ServerPeer::getImage(string username, string token, std::string imageID){
	if(serviceDiscoveryClient->auth(username, token)){
		cout << "SERVERPEER::getImage!";
	//	std::string dataImage = "";
		string imageLocation = myImagesPath + "steg_" + imageID;
		std::ifstream fin(imageLocation, std::ios::in | std::ios::binary);
		std::ostringstream oss;
		oss << fin.rdbuf();
		std::string dataImage(oss.str());
        return dataImage;
	}
    string temp = "";
    return temp;
}

void ServerPeer::decrementPeerImage(std::string userID, std::string imageID){
    string extract_command = "steghide extract -p 123 -sf " + loadedImagesPath + userID + "/steg_" + imageID + " -xf " + loadedImagesPath + userID + "/" + "data_" + imageID;
    system(extract_command.c_str());

    extract_command = "steghide extract -p 123 -sf " + loadedImagesPath + userID + "/data_" + imageID + " -xf " + loadedImagesPath + userID + "/" + imageID + ".data.txt";
    system(extract_command.c_str());

    string line;
    ifstream viewData(imageID + ".data.txt");
    map<std::string, int> data;

    if(viewData.is_open()){
        while(getline(viewData,line)){
            int delimiter = line.find(';');
            string username = line.substr(0, delimiter);
            string views = line.substr(delimiter+1, string::npos);
            data[username] = stoi(views);
        }
        viewData.close();
    }
    int count = data[userID];
    count--;

    if(count == 0){
        string remove_old_command = "rm " + loadedImagesPath + userID + "/" + imageID + ".data.txt";
        system(remove_old_command.c_str());
        remove_old_command = "rm data_" + loadedImagesPath + userID + "/" + imageID;
        system(remove_old_command.c_str());
        remove_old_command = "rm steg_" + loadedImagesPath + userID + "/" + imageID;
        system(remove_old_command.c_str());
        return;
    }
    else
        data[userID] = count;

    string remove_old_command = "rm " + imageID + ".data.txt";
    system(remove_old_command.c_str());

    ofstream newData(imageID + ".data.txt");
    if(newData.is_open()){
        for (std::map<string,int>::iterator it=data.begin(); it!=data.end(); ++it)
            newData << it->first << ";" << it->second << '\n';
    }

    string create_new_command;
    create_new_command = "steghide embed -p 123 -cf " + loadedImagesPath + userID + "/" + imageID + " -ef " + loadedImagesPath + userID + "/" + imageID + ".data.txt -sf " + loadedImagesPath + userID + "/" + "data_" + imageID;
    system(create_new_command.c_str());
    create_new_command = "steghide embed -p 123 -cf " + loadedImagesPath + userID + "/" + ".default.jpeg -ef " + loadedImagesPath + userID + "/" + "data_" + imageID + " -sf " + loadedImagesPath + userID + "/" + "steg_" + imageID;
    system(create_new_command.c_str());

    remove_old_command = "rm " + loadedImagesPath + userID + "/" + imageID + ".data.txt";
    system(remove_old_command.c_str());
    remove_old_command = "rm data_" + loadedImagesPath + userID + "/" + imageID;
    system(remove_old_command.c_str());
}

void ServerPeer::updateLocalViews(std::string userID, std::string imageID, int count){
    string extract_command = "steghide extract -p 123 -sf " + myImagesPath + "steg_" + imageID + " -xf " + myImagesPath + "data_" + imageID;
    system(extract_command.c_str());

    extract_command = "steghide extract -p 123 -sf " + myImagesPath + "data_" + imageID + " -xf " + myImagesPath + imageID + ".data.txt";
    system(extract_command.c_str());

	string line;
	ifstream viewData(imageID + ".data.txt");
	map<std::string, int> data;

	if(viewData.is_open()){
		while(getline(viewData,line)){
			int delimiter = line.find(';');
			string username = line.substr(0, delimiter);
            string views = line.substr(delimiter+1, string::npos);
			data[username] = stoi(views);
		}
		viewData.close();
	}

	if(count == 0)
		data.erase(userID);
	else
		data[userID] = count;

	string remove_old_command = "rm " + imageID + ".data.txt";
	system(remove_old_command.c_str());

	ofstream newData(imageID + ".data.txt");
	if(newData.is_open()){
		for (std::map<string,int>::iterator it=data.begin(); it!=data.end(); ++it)
		    newData << it->first << ";" << it->second << '\n';
	}

    remove_old_command = "rm " + myImagesPath + "steg_" + imageID;
    system(remove_old_command.c_str());

	string create_new_command;
	create_new_command = "steghide embed -p 123 -cf " + myImagesPath + imageID + " -ef " + myImagesPath + imageID + ".data.txt -sf " + myImagesPath + "data_" + imageID;
	system(create_new_command.c_str());
    create_new_command = "steghide embed -p 123 -cf " + myImagesPath + ".default.jpeg -ef " + myImagesPath + "data_" + imageID + " -sf " + myImagesPath + "steg_" + imageID;
	system(create_new_command.c_str());

    remove_old_command = "rm " + myImagesPath + imageID + ".data.txt";
    system(remove_old_command.c_str());
    remove_old_command = "rm " + myImagesPath + "data_" + imageID;
    system(remove_old_command.c_str());
    remove_old_command = "rm " + myImagesPath + imageID;
    system(remove_old_command.c_str());

}

void ServerPeer::updateViews(std::string username, std::string token, std::string imageID, int count){
	cout << "SERVERPEER::updateViews!";
	if(serviceDiscoveryClient->auth(username, token))
		updateLocalViews(username, imageID, count);
}

void ServerPeer::addRequester(string username, string token, string imageID, int views){
	cout << "SERVERPEER::addRequester!";
	if(serviceDiscoveryClient->auth(username, token)){
		requesters[imageID][username] = views;
	}
}

Message* ServerPeer::doOperation(Message* _message){
	cout << "SERVERPEER::doOperation!";
	Message* reply_message = new Message(Reply, _message->getRPCId());

    vector<Parameter> args;
    vector<Parameter> reply_args;

    int operation = _message->getOperation();

    MessageDecoder::decode(_message, args);

    switch(operation){
    case 7:{
    	std::vector<std::string> imageList = getListofImages(args[0].getString(), args[1].getString());
    	Parameter arg1;
		arg1.setVectorString(imageList);
		reply_args.push_back(arg1);
    	}
    	break;
    case 8:{
    	addRequester(args[0].getString(), args[1].getString(), args[2].getString(), args[3].getInt());
    	}
    	break;
    case 11:{
    	addApprovedImage(args[0].getString(), args[1].getString(), args[2].getString(), args[3].getInt(), args[4].getBoolean());
    	}
    	break;
    case 9:{
    	updateViews(args[0].getString(), args[1].getString(), args[2].getString(), args[3].getInt());
    	}
    	break;
    case 12:{
    	string imageData = getImage(args[0].getString(), args[1].getString(), args[2].getString());
    	Parameter arg1;
    	arg1.setString(imageData);
    	reply_args.push_back(arg1);
    	}
    break;
    }


	MessageDecoder::encode(*reply_message, reply_args, operation, Reply);
    return reply_message;
}

void ServerPeer::addApprovedImage(string username, string token, string imagename, int views, bool approve){
	if(serviceDiscoveryClient->auth(username, token)){
		if(approve)
			approvedImages[username][imagename] = views;
	}
}

void ServerPeer::removeRequester(string username, string imageName){
	requesters[imageName].erase(username);
    if(requesters[imageName].empty())
        requesters.erase(imageName);

}

void ServerPeer::writePeerImage(string username, string imagename, string image){
	string dir_path = loadedImagesPath + username;
	mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    string imagePath = dir_path + "/steg_" + imagename;
	std::ofstream fin(imagePath, std::ios::out | std::ios::binary);
	cout << image.size();
	fin << image;
	fin.close();
	requested[imagename].erase(username);
}

void ServerPeer::addRequested(string username, string imageName, int views){
	requested[imageName][username] = views;
}

vector<string> ServerPeer::getRequesters(string imageName){
	std::vector<std::string> req;
	std::map<string, int> imageRequesters = requesters[imageName];

	for (std::map<string,int>::iterator it=imageRequesters.begin(); it!=imageRequesters.end(); ++it)
		req.push_back(it->first);

	return req;
}

vector<string> ServerPeer::getRequestedImages(){
	std::vector<std::string> req;
	for (std::map<string,map<string, int>>::iterator it=requesters.begin(); it!=requesters.end(); ++it)
		req.push_back(it->first);

	return req;
}

vector<string> ServerPeer::whoCanView(string imageName){
	std::vector<std::string> viewers;
    string extract_command = "steghide extract -p 123 -sf " + myImagesPath + "steg_" + imageName + " -xf " + myImagesPath + "data_" + imageName;
    system(extract_command.c_str());

    extract_command = "steghide extract -p 123 -sf " + myImagesPath + "data_" + imageName + " -xf " + myImagesPath + imageName + ".data.txt";
    system(extract_command.c_str());

	string line;
	ifstream viewData(imageName + ".data.txt");
	map<std::string, int> data;

	if(viewData.is_open()){
		while(getline(viewData,line)){
			int delimiter = line.find(';');
			string username = line.substr(0, delimiter);
			viewers.push_back(username);
		}
		viewData.close();
	}

    string remove_old_command = "rm " + myImagesPath + imageName + ".data.txt";
    system(remove_old_command.c_str());
    remove_old_command = "rm " + myImagesPath + "data_" + imageName;
    system(remove_old_command.c_str());

	return viewers;
}

int ServerPeer::viewsCount(string username, string imageName){
    int views = 0;
    string extract_command = "steghide extract -p 123 -sf " + myImagesPath + "steg_" + imageName + " -xf " + myImagesPath + "data_" + imageName;
	system(extract_command.c_str());

    extract_command = "steghide extract -p 123 -sf " + myImagesPath + "data_" + imageName + " -xf " + myImagesPath + imageName + ".data.txt";
	system(extract_command.c_str());

	string line;
	ifstream viewData(imageName + ".data.txt");
	map<std::string, int> data;

    if(viewData.is_open()){
        while(getline(viewData,line)){
            int delimiter = line.find(';');
            string username = line.substr(0, delimiter);
            string views = line.substr(delimiter+1, string::npos);
            data[username] = stoi(views);
        }
        viewData.close();
    }

    map<std::string, int>::const_iterator it = data.find(username);
    if(it!=data.end())
        views = data[username];

    string remove_old_command = "rm " + myImagesPath + imageName + ".data.txt";
	system(remove_old_command.c_str());
    remove_old_command = "rm " + myImagesPath + "data_" + imageName;
	system(remove_old_command.c_str());
	return views;
}

cv::Mat ServerPeer::readPeerImage(string& username, string& imagename){
	cv::Mat image = extractImage(username, imagename, 0);
	decrementPeerImage(username, imagename);
	approvedImages[username][imagename]--;
	return image;
}

vector<string> ServerPeer::getListOfMyImages(){
	cout << "In ServerPeer" << endl;
	return loadFileNames(myImagesPath);
}

cv::Mat ServerPeer::getMyImage(string& imagename){
	cv::Mat image = extractImage("", imagename, 1);
	return image;
}

int ServerPeer::allowedViews(string username, string imageName){
	map<string,map<string, int>>::const_iterator it = approvedImages.find(username);
	if(it!=approvedImages.end()){
		return approvedImages[username][imageName];
	}
	else
		return 0;
}

cv::Mat ServerPeer::extractImage(string username, string imagename, bool owned){
	string imagePath;
	if(owned)
		imagePath = myImagesPath + "steg_" +  imagename;
	else
		imagePath = loadedImagesPath + username + "/steg_" + imagename;

	string extract_command = "steghide extract -p 123 -sf " + imagePath + " -xf " + myImagesPath + "data_" + imagename;
	system(extract_command.c_str());

	string _image = myImagesPath + "data_" + imagename;
	cv::Mat image = cv::imread(_image, cv::IMREAD_UNCHANGED);
	string remove_command = "rm " + myImagesPath + "data_" + imagename;
	system(remove_command.c_str());
	return image;
}

void ServerPeer::writeMyImage(string imagePath){
	std::size_t found = imagePath.find_last_of("/\\");
	string imgName = imagePath.substr(found+1);

    string copy_command = "cp " + imagePath + " ./MyImages/" + imgName;
	system(copy_command.c_str());

    std::size_t format = imgName.find(".png");
    if(format != std::string::npos){
        string convert_command = "convert ./MyImages/" + imgName + " ./MyImages/" + imgName.substr(0, format) + ".jpg";
        system(convert_command.c_str());

        string remove_old_format = "rm ./MyImages/" + imgName;
        system(remove_old_format.c_str());
        imgName = imgName.substr(0, format) + ".jpg";
    }

    string make_data_command = "echo "" > ./MyImages/" + imgName +".data.txt";
	system(make_data_command.c_str());

	string create_new_command;
    create_new_command = "steghide embed -p 123 -cf ./MyImages/" + imgName + " -ef ./MyImages/" + imgName + ".data.txt -sf ./MyImages/data_" + imgName;
    cout << create_new_command << endl;
    system(create_new_command.c_str());
    create_new_command = "steghide embed -p 123 -cf ./MyImages/.default.jpeg -ef ./MyImages/data_" + imgName + " -sf ./MyImages/steg_" + imgName;
	system(create_new_command.c_str());

    string remove_old_command = "rm ./MyImages/" + imgName + ".data.txt";
	system(remove_old_command.c_str());
    remove_old_command = "rm ./MyImages/data_" + imgName;
	system(remove_old_command.c_str());
    remove_old_command = "rm ./MyImages/" + imgName;
    system(remove_old_command.c_str());
}
