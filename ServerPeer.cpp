#include "ServerPeer.h"
#include "Parameter.h"
#include "MessageDecoder.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ServerPeer::ServerPeer(char* _listen_hostname, int _listen_port):Server(_listen_hostname, _listen_port){
	system("mkdir MyImages");
	system("mkdir LoadedImages");
}

ServerPeer::~ServerPeer(){

}

std::vector<std::string> ServerPeer::getListofImages(std::string token, std::string userID){
	//TODO: AUTHENTICATE WITH SERVER DISCOVERY
	return imageList;
}

std::string ServerPeer::getImage(std::string token, std::string userID, std::string imageID){
	//TODO: AUTHENTICATE WITH SERVER DISCOVERY
	streampos size;
	char * memblock;
	string imageLocation = "MyImages" + imageID;
	ifstream file (imageLocation, ios::in|ios::binary|ios::ate);
	if (file.is_open()){
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		string image(memblock, size);
		delete[] memblock;
		return image;
	}
	else return NULL;
}

void ServerPeer::updateViews(std::string token, std::string userID, int count){

}

void ServerPeer::revokeViews(std::string token, std::string userID){

}

Message* ServerPeer::doOperation(Message* _message){
	Message reply_message(Reply);

    vector<Parameter> args;
    vector<Parameter> reply_args;

    int operation = _message->getOperation();
    MessageType = _message->getMessageType();

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

	MessageDecoder::encode(reply_message, reply_args, operation, Reply);
    return reply_message;
}
