/*
 * Message.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: ewais
 */
#include "Message.h"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

Message::Message(MessageType type){
	rpc_id = rpc_count++;
	message_type = type;
	message_size = 0;
	operation = 0;
	message = NULL;
	parts_num = 1;
	part_num = 0;
}
Message::Message(Message* big,int _parts_num, int _part_num, int _size, int _mx_size):
		message_type(big->message_type),
		rpc_id(big->rpc_id),
		parts_num(_parts_num),
		part_num(_part_num),
		operation(big->operation),
		message_size(_size){
	message = new char[message_size];
	memcpy(message, big->message+_mx_size*part_num, message_size);
}
Message::Message(char* data){
	if(data==NULL)
		cout<<"data corrupted or NULL\n";
	message_type = (MessageType) ntohl((uint32_t &)data);
	rpc_id = ntohl((uint32_t &)data+4);
	parts_num = ntohl((uint32_t &)data+8);
	part_num = ntohl((uint32_t &)data+12);
	operation = ntohl((uint32_t &)data+16);
	message = NULL;
	message_size = 0;
	if(message_type != Ack)
		return;
	message_size = Base64_Marshal::Base64decode((char *)message, data+24);
	cout<<message_size<<" part: "<<part_num<<endl;
}

Message::Message(vector<Message*>& parts){
	if(parts.size()==0){
		cout<<"vector is empty...cannot construct Message\n";
		return;
	}
	message_type = parts[0]->message_type;
	rpc_id = parts[0]->rpc_id;
	for(int i=0;i<parts.size();i++)
		if(parts[i]->rpc_id!=rpc_id){
			cout<<"not the same rpc_id\n";
			return;
		}
	operation = parts[0]->operation;
	parts_num = 1;
	part_num = 0;
	message_size = 0;
	int siz=0;
	for(int i=0;i<parts.size();i++){
		message_size+=parts[i]->message_size;;
		siz = max(siz, (int)parts[i]->message_size);
	}
	message = new char[message_size];
	for(int i=0;i<parts.size();i++){
		memcpy(message+parts[i]->part_num*siz, parts[i]->message, parts[i]->message_size);
		delete parts[i];
	}


}

char * Message::marshal(int &size){
	char* to_send = NULL;
	size = message_size + 28;
	to_send = new char[size];
	int& type=(int&)to_send, id= (int&)to_send+4, parts = (int&)to_send+8, part = (int&)to_send+12,
			op = (int&) to_send+16, siz = (int&) to_send+20;
	type = htonl(message_type);
	id = htonl(rpc_id);
	parts = htonl(parts_num);
	part = htonl(part_num);
	op = htonl(operation);
	siz = htonl (message_size);
	if(message_size){
		int encoded_bytes = Base64_Marshal::Base64encode(to_send+24 ,(const char*) message, message_size);
		std::cout<<encoded_bytes<<" "<<size-20<<std::endl;
	}

	return to_send;
}

vector<Message*> Message::split(int max_size){
	vector<Message*> res;
	int parts = ceil(message_size/(float)max_size);
	for (int i=0;i<parts;i++)
		res.push_back(new Message(this,parts, i ,(i!=parts-1)?max_size:message_size-i*max_size, max_size));

//	while(sent < message_size){
//		int trials = 3;		//in case of local send error
//		int siz = (message_size - sent > max_size)? max_size: message_size-sent;
//		res.push_back(new Message(this, max_size));
//		sent+=siz;
//	}
	return res;
}


int Message::getPartsNum(){
	return parts_num;
}

int Message::getPartNum(){
	return part_num;
}

bool Message::isComplete(){
	return part_num == parts_num-1;
}
MessageType Message::getMessageType(){
	return message_type;
}

char* Message::getMessage(){
	return message;
}

size_t Message::getMessageSize(){
	return message_size;
}

int Message::getOperation(){
	return operation;
}

int Message::getRPCId(){
	return rpc_id;
}

void Message::setOperation(int op){
	operation = op;
}

void Message::setMessage(const char* _message, size_t _message_size){
	message_size = _message_size;
	if(message !=NULL)
		delete [] message;
	message = new char[message_size];
	memcpy(message, _message, message_size);
}

void Message::setMessageType(enum MessageType _message_type){
	message_type = _message_type;
}

void Message::print(){
	printf("RPC Id: %d\nparts number: %d\npart number:%d\noperation: %d\nmessage_size: %d\n",rpc_id,parts_num,part_num,operation,message_size);
	cout<<message<<endl;
	cout<<"end print//////////////////////////////////////"<<endl;
}

Message::~Message(){
	delete []message;
}

