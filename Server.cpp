#include "Server.h"
#include <iostream>



Server::Server(char* _listen_hostname, int _listen_port){
	udpServerSocket = new UDPServerSocket();
	udpServerSocket->initializeServer(_listen_hostname, _listen_port);
}
/*------------------------------------------*/
/*Message operation
Message* Server::getRequest(){
	udpServerSocket().readFromSocketWithBlock(char* buffer);
}

Message* Server::doOperation(){

}

void Server::sendReply(Message* _message){
	_message.setMessageType(1);
	writeToSocket();
}*/
/*------------------------------------------*/
//char operations

char* Server::getRequest(){
	char* buffer = new char[1024];
	udpServerSocket->readFromSocketWithBlock(buffer, 1024);
	std::cout << buffer << std::endl;
	return buffer;
}

char* Server::doOperation(char* _message){
	for(int i = 0; i < 1024; i++){
		if(_message[i] == NULL){
			_message[i] = ':';
			_message[i+1] = ':';
			_message[i+2] = 'R';
			_message[i+3] = NULL;
			break;
		}
	}
	
	return _message;
}

void Server::sendReply(char* _message){
	udpServerSocket->writeToSocket(_message, 1024);
}

void Server::serveRequest(){
	while(1){
		char* _message = getRequest();
		if (_message[0] == 'q' && _message[1] == NULL)
			break;

		_message = doOperation(_message);
		sendReply(_message);
	}
}

Server::~Server(){

}
