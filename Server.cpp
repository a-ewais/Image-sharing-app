#include <string>
#include "Server.h"

using namespace std;

Server::Server(char* _listen_hostname, int _listen_port){
	udpServerSocket = new UDPServerSocket(_listen_hostname, _listen_port);
}

Server::~Server(){
	delete udpServerSocket;
}

void Server::sendReply(Message* _message){
	udpServerSocket->sendReply(_message);
}

Message* Server::getRequest(){
	return udpServerSocket->getRequest();
}

sockaddr_in Server::getMyAddr(){
	return udpServerSocket->getMyAddr();
}
pair<string, int> Server::getMyAddrPair(){
	return udpServerSocket->getMyAddrPair();
}
void Server::startListen(){
	while(1){
		if(udpServerSocket->readyRequest()){
			Message* _message = getRequest();
			if(_message!=NULL){
				_message = doOperation(_message);
				sendReply(_message);
			}
		}
	}
}
