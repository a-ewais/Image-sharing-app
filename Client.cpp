/*
 * Client.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: ewais
 */
#include<iostream>
#include<vector>
#include"Client.h"
using namespace std;
Client::Client(char* _hostname, int _port){
	udpSocket = new UDPClientSocket(_hostname, _port);
}

Message * Client::execute(Message* message){

	Message* temp = udpSocket->sendWaitForReply(message, 30);
	if(temp==NULL)
		cout<<"failed to send message..network error\n";
	else
		cout<<"message number: "<<temp->getRPCId()<<" sent successfully\n";
	return temp;
}

Client::~Client(){
	delete udpSocket;
}


