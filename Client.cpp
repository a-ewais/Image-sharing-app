/*
 * Client.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: ewais
 */

#include"Client.h"

Client::Client(char * _hostname, int _port){
	udpSocket = new UDPClientSocket();
	udpSocket->initializeClient(_hostname, _port);
}

char * Client::execute(char * _message, int _size){
//	int size = 1024;
//	char * message = new char[size];
//	Message * reply = new Message(message, size);
//	int flag = udpSocket->writeToSocket((char*)_message->message, _message->message_size);
//	if(flag<0){
//		delete [] message;
//		delete reply;
//		perror("Failed to send the message\n");
//		return NULL;
//	}
//	int s = udpSocket->readFromSocketWithBlock(message,size);
//
//	reply->setMessage(message, size);
//	delete [] message;
//	return reply;
	int size = 1024;
	char * reply = new char [size];
	int flag = udpSocket->writeToSocket(_message,_size);
	if(flag<0){
		delete [] reply;
		perror("Failed to send the message\n");
		return NULL;
	}
	flag = udpSocket->readFromSocketWithBlock(reply, size);
	if(flag<0)
		perror("Failed to receive reply");
	return reply;
}

Client::~Client(){
	delete udpSocket;
}


