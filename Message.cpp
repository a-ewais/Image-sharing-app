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

Message::Message(Message* big, int _part_num, int _size ):
		message_type(big->message_type),
		rpc_id(big->rpc_id),
		parts_num(big->parts_num),
		part_num(_part_num),
		operation(big->operation),
		message_size(_size){
	message = new char[message_size];
	memcpy((char*)message, (char*)big->message+_size*part_num, message_size);
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
	message_size = Base64decode((char *)message, data+24);
	cout<<message_size<<" part: "<<part_num<<endl;
}

Message::Message(vector<Message*> parts){
	if(parts.size()==0){
		cout<<"vector is empty...cannot construct Message\n";
		return;
	}
	message_type = parts[0]->message_type;
	rpc_id = parts[0]->rpc_id;
	operation = parts[0]->operation;
	parts_num = 1;
	part_num = 0;
	message_size = 0;
	int siz=0;
	for(int i=0;i<parts.size();i++){
		message_size = parts[i]->message_size;
		siz = max(siz, (int)parts[i]->message_size);
	}
	message = new char[message_size];
	for(int i=0;i<parts.size();i++)
		memcpy((char *) message+parts[i]->part_num*siz, (char *)parts[i]->message, parts[i]->message_size);
}

char * Message::marshal(int &size){
	char* to_send = NULL;
	size = message_size + 24;
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
		int encoded_bytes = Base64encode(to_send+24 ,(const char*) message, message_size);
		std::cout<<encoded_bytes<<" "<<size-20<<std::endl;
	}

	return to_send;
}

vector<Message*> Message::split(int max_size){
	vector<Message*> res;
	int parts = ceil(message_size/max_size);
	for (int i=0;i<parts;i++)
		res.push_back(new Message(this, i ,(i!=parts-1)?max_size:message_size-i*max_size));

//	while(sent < message_size){
//		int trials = 3;		//in case of local send error
//		int siz = (message_size - sent > max_size)? max_size: message_size-sent;
//		res.push_back(new Message(this, max_size));
//		sent+=siz;
//	}
	return res;
}





