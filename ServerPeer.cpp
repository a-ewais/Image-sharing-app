#include "ServerPeer.h"
#include "Parameter.h"
#include "MessageDecoder.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

using namespace std;

ServerPeer::ServerPeer(char* _listen_hostname, int _listen_port, Client* _serviceDiscoveryClient):Server(_listen_hostname, _listen_port){
	const int dir_err_owned = mkdir("MyImages", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	const int dir_err_loaded = mkdir("LoadedImages", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	myPath = get_current_dir_name();
	myImagesPath = myPath + "/MyImages/";
	loadedImagesPath = myPath + "/LoadedImages/";
	serviceDiscoveryClient = _serviceDiscoveryClient;
}

ServerPeer::~ServerPeer(){

}

vector<string> ServerPeer::loadFileNames(string path){
	std::vector<std::string> results;
	DIR* dir_point = opendir(path.c_str());
	dirent* entry = readdir(dir_point);
	while (entry){									// if !entry then end of directory
		if (entry->d_type == DT_REG){		// if entry is a regular file
			std::string fname = entry->d_name;	// filename
			results.push_back(fname);		// add filename to results vector
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

std::string ServerPeer::getImage(std::string username, std::string token, std::string imageID){
	cout << "SERVERPEER::getImage!";
	if(serviceDiscoveryClient->auth(username, token)){
		string imageLocation = myImagesPath + imageID;
		cv::Mat image = cv::imread(imageLocation, CV_LOAD_IMAGE_COLOR);
		unsigned char* dataImage = image.data;
		std:: string s(dataImage);
		return s;
//		streampos size;
//		char * memblock;
//		string imageLocation = myImagesPath + imageID;
//		ifstream file (imageLocation, ios::in|ios::binary|ios::ate);
//		if (file.is_open()){
//			size = file.tellg();
//			memblock = new char [size];
//			file.seekg (0, ios::beg);
//			file.read (memblock, size);
//			file.close();
//			string image(memblock, size);
//			delete[] memblock;
//			return image;
//		}else return NULL;
	}
	else return NULL;
}

void ServerPeer::updateViews(std::string token, std::string userID, int count){
	cout << "SERVERPEER::updateViews!";

}

void ServerPeer::revokeViews(std::string token, std::string userID){
	cout << "SERVERPEER::revokeViews!";

}

Message* ServerPeer::doOperation(Message* _message){
	cout << "SERVERPEER::doOperation!";
	Message* reply_message = new Message(Reply);

    vector<Parameter> args;
    vector<Parameter> reply_args;

    int operation = _message->getOperation();
    //MessageType _type = _message->getMessageType();

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
    	string image = getImage(args[0].getString(), args[1].getString(), args[2].getString());
    	Parameter arg1;
    	arg1.setString(image);
    	reply_args.push_back(arg1);
    }
    	break;
    case 9:{

    }
    	break;
    }

	MessageDecoder::encode(*reply_message, reply_args, operation, Reply);
    return reply_message;
}

void ServerPeer::writePeerImage(string& username,string &imagename, cv::Mat& image){
	const int dir_err_owned = mkdir(username.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	string imagePath = loadedImagesPath + username + "/" + imagename;
	cv::imwrite(imagePath, image);
}

cv::Mat ServerPeer::readPeerImage(string& username, string& imagename){
	string imagePath = loadedImagesPath + imagename;
	cv::Mat image = cv::imread(imagePath, CV_LOAD_IMAGE_COLOR);
	return image;
}

vector<string> ServerPeer::getListOfMyImages(){
	return loadFileNames(myImagesPath);
}

cv::Mat ServerPeer::getMyImage(string& imagename){
	string imagePath = myImagesPath + imagename;
	cv::Mat image = cv::imread(imagePath, CV_LOAD_IMAGE_COLOR);
	return image;
}

