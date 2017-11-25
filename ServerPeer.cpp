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
	const int dir_err_owned = mkdir("MyImages", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	const int dir_err_loaded = mkdir("LoadedImages", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	myPath = get_current_dir_name();
	myImagesPath = myPath + "/MyImages/";
	loadedImagesPath = myPath + "/LoadedImages/";
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
			if(fname != "default.jpeg")
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

std::string ServerPeer::getImage(std::string username, std::string token, std::string imageID){
	cout << "SERVERPEER::getImage!";
	std::string dataImage = "";
	if(serviceDiscoveryClient->auth(username, token)){
		string imageLocation = myImagesPath + "steg_" + imageID;
//		cv::Mat image = cv::imread(imageLocation, cv::IMREAD_UNCHANGED);
//		unsigned char* dataImage = image.data;
//		std::string _dataImage(reinterpret_cast<char*>(dataImage));
		std::ifstream fin(imageLocation, std::ios::in | std::ios::binary);
		std::ostringstream oss;
		oss << fin.rdbuf();
		std::string dataImage(oss.str());
		return dataImage;
	}
	else return dataImage;
}

void ServerPeer::updateLocalViews(std::string userID, std::string imageID, int count){
	string extract_command = "steghide extract -p 123 -sf " + myImagesPath + "steg_" + imageID;
	system(extract_command.c_str());

	extract_command = "steghide extract -p 123 -sf " + myImagesPath + "data_" + imageID;
	system(extract_command.c_str());

	string line;
	ifstream viewData(imageID + ".data.txt");
	map<std::string, int> data;

	if(viewData.is_open()){
		while(getline(viewData,line)){
			int delimiter = line.find(';');
			string username = line.substr(0, delimiter);
			string views = line.substr(delimiter, string::npos);
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

	string create_new_command;
	create_new_command = "steghide embed -p 123 -cf " + imageID + " -ef " + imageID + ".data.txt -sf data_" + imageID;
	system(create_new_command.c_str());
	create_new_command = "steghide embed -p 123 -cf default.jpeg -ef data_" + imageID + " -sf steg_" + imageID;
	system(create_new_command.c_str());

}

void ServerPeer::updateViews(std::string username, std::string token, std::string imageID, int count){
	cout << "SERVERPEER::updateViews!";
	if(serviceDiscoveryClient->auth(username, token))
		updateLocalViews(username, imageID, count);
}

Message* ServerPeer::doOperation(Message* _message){
	cout << "SERVERPEER::doOperation!";
	Message* reply_message = new Message(Reply, _message->getRPCId());

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
    	updateViews(args[0].getString(), args[1].getString(), args[2].getString(), args[3].getInt());
    }
    	break;
    }

	MessageDecoder::encode(*reply_message, reply_args, operation, Reply);
    return reply_message;
}

void ServerPeer::writePeerImage(string& username,string &imagename, string& image){
	string dir_path = loadedImagesPath + username;
	const int dir_err_owned = mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	string imagePath = dir_path + "/" + imagename;
	std::ofstream fin(imagePath, std::ios::out | std::ios::binary);
	cout << image.size();
	fin << image;
	fin.close();
}

cv::Mat ServerPeer::readPeerImage(string& username, string& imagename){
	cv::Mat image = extractImage(username, imagename, 0);
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
