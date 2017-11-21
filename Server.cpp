#include "Server.h"

Server::Server(char* _listen_hostname, int _listen_port){
	udpServerSocket = new UDPServerSocket(_listen_hostname, _listen_port);
}

Server::~Server(){
	delete udpServerSocket;
}

void Server::sendReply(Message* _message){
	udpServerSocket->sendReply(_message);
}

//bool Server::sendReplyWithAck(Message* _message){
//	bool received = udpServerSocket->sendReplyWaitAck(_message, 3);
//	if(!received)
//		printf("No acknowledgment received. Network Error!\n");
//
//	return received;
//}

Message* Server::getRequest(){
	return udpServerSocket->getRequest();
}

sockaddr_in Server::getMyAddr(){
	return udpServerSocket->getMyAddr();
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
