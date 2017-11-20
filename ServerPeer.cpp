#include "ServerPeer.h"
#include "Parameter.h"
#include "MessageDecoder.h"

using namespace std;

ServerPeer::ServerPeer(char* _listen_hostname, int _listen_port):Server(_listen_hostname, _listen_port){
}

ServerPeer::~ServerPeer(){

}

std::vector<std::string> ServerPeer::getListofImages(std::string token, std::string userID){
	//TODO: AUTHENTICATE WITH SERVER DISCOVERY
	return imageList;
}

std::string ServerPeer::getImage(std::string token, std::string userID, std::string imageID){
	//TODO: LOAD IMAGES
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
    case 7:
    	break;
    case 8:
    	break;
    case 9:
    	break;
    }

	MessageDecoder::encode(reply_message, reply_args, operation, Reply);
    return reply_message;
}
