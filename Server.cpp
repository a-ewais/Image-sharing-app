#include "Server.h"

Server::Server(char* _listen_hostname, int _listen_port){
	udpServerSocket().initializeServer(_listen_hostname, _listen_port);
}
/*------------------------------------------*/
//Message operation
Message* Server::getRequest(){
	udpServerSocket().readFromSocketWithBlock(char* buffer);
}

Message* Server::doOperation(){

}

void Server::sendReply(Message* _message){
	_message.setMessageType(1);
	writeToSocket();
}
/*------------------------------------------*/
//char operations

char* Server::getRequest(){
	char* buffer = NULL;
	udpServerSocket().readFromSocketWithBlock(buffer, 1024);
	return buffer;
}

char* Server::doOperation(char* _message){
	for(int i = 0; i < 1024; i++){
		if(_message[i] == '\0'){
			_message[i] = '9';
			_message[i+1] = '\0';
		}
	}
	
	return _message;
}

void Server::sendReply(char* _message){
	udpServerSocket().writeToSocket(_message, 1024);
}

void Server::serveRequest(){
	char* _message = getRequest();
	_message = doOperation(_message);
	sendReply(_message);
}

Server::~Server(){

}
