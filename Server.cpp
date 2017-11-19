#include "Server.h"

Server::Server(char* _listen_hostname, int _listen_port){
	udpServerSocket = new UDPServerSocket(_listen_hostname, _listen_port);
}

Server::~Server(){
	delete udpServerSocket;
}

Message* Server::doOperation(Message* _message){

}

void Server::sendReply(Message* _message){
	udpServerSocket->sendReply(_message);
}
bool Server::sendReplyWithAck(Message* _message){
	//return udpServerSocket->sendReplyWaitAck(_message, 3);
	return 1;
}

Message* Server::getRequest(){
	return udpServerSocket->getRequest();
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
