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
	udpSocket = new UDPClientSocket();
	udpSocket->initializeClient(_hostname, _port);
}

char* Client::execute(char* _message, int _size){
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
	char* reply = new char[size];
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

Message *  Client::execute(Message* message){
	int flag;
	if((flag=send_message(message))==-1){
		cout<<"failed to send message..network error\n";
		return NULL;
	}
	else{
		cout<<"message number: "<<flag<<" sent successfully\n";
		return get_respond(flag);
	}
}

bool received_ack(int id){
	//TODO: implement to check for ack for each message.
	return true;
}

Message* Client::get_respond(int id){
	vector<Message *> res;
	bool f = true;
	while(f){
		char* incoming = new char[MAX_DATAGRAM_SIZE];
		udpSocket->readFromSocketWithTimeout(incoming, MAX_DATAGRAM_SIZE, 10000);
		res.push_back(new Message(incoming));
		if(res[res.size()-1]->get_parts_num()==res.size())
			f = false;
	}
	return new Message(res);
}

int Client::send_message(Message* message){
	int message_id= message->getRPCId();
	vector<Message*> parts;
	if(sizeof(message)> MAX_DATAGRAM_SIZE)
		parts = message->split(MAX_DATAGRAM_SIZE);
	else
		parts.push_back(message);
	for(int i=0;i<parts.size();i++)
	{	int temp = 0;
		char* x = parts[i]->marshal(temp);
		udpSocket->writeToSocket(x,temp);
	}
	if(!received_ack(message_id))
		return -1;
	return message_id;
}
Client::~Client(){
	delete udpSocket;
}


