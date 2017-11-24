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
void* Server::startListen(void* arg){
	Server* me = (Server*)arg;

	while(1){
		if(me->udpServerSocket->readyRequest()){
			Message* _message = me->getRequest();
			if(_message!=NULL){
				_message = me->doOperation(_message);
				me->sendReply(_message);
			}
		}
	}
}
