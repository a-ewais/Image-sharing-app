#include "Message.h"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

int Message::rpc_count = 0;
pthread_mutex_t Message::rpc_mutex = PTHREAD_MUTEX_INITIALIZER;

Message::Message(MessageType type){
	rpc_id = Message::getNewRPC();
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
	int size = Base64_Marshal::Base64decode_len(data)-1;
	char * temp = new char [size];
	Base64_Marshal::Base64decode(temp, data);
	uint32_t* type = reinterpret_cast<uint32_t*>(temp);
	uint32_t* id = reinterpret_cast<uint32_t*>(temp+4);
	uint32_t* parts = reinterpret_cast<uint32_t*>(temp+8);
	uint32_t* part = reinterpret_cast<uint32_t*>(temp+12);
	uint32_t* op = reinterpret_cast<uint32_t*>(temp+16);
	uint32_t* siz = reinterpret_cast<uint32_t*>(temp+20);
	message_type = (MessageType) ntohl(*type);
	rpc_id = ntohl(*id);
	parts_num = ntohl(*parts);
	part_num = ntohl(*part);
	operation = ntohl(*op);
	message_size = ntohl(*siz);
	if(message_size){
		message = new char[message_size];
		memcpy(message, temp+24, message_size);
	}
	delete [] temp;
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

int Message::getNewRPC(){
	pthread_mutex_lock(&Message::rpc_mutex);
	int t = rpc_count++;
	pthread_mutex_unlock(&Message::rpc_mutex);
	return t;
}

char * Message::marshal(int &size){

	size = Base64_Marshal::Base64encode_len(message_size+24);
	char* to_send = new char[size];
	char* s = new char[message_size+24];
	uint32_t* type = reinterpret_cast<uint32_t*>(s);
	uint32_t* id = reinterpret_cast<uint32_t*>(s+4);
	uint32_t* parts = reinterpret_cast<uint32_t*>(s+8);
	uint32_t* part = reinterpret_cast<uint32_t*>(s+12);
	uint32_t* op = reinterpret_cast<uint32_t*>(s+16);
	uint32_t* siz = reinterpret_cast<uint32_t*>(s+20);
	*type = htonl(message_type);
	*id = htonl(rpc_id);
	*parts = htonl(parts_num);
	*part = htonl(part_num);
	*op = htonl(operation);
	*siz = htonl (message_size);
	memcpy(s+24, message, message_size);
	int encoded_bytes = Base64_Marshal::Base64encode(to_send ,(const char*) s, message_size+24);
	delete [] s;
	size = encoded_bytes;
	return to_send;
}

vector<Message*> Message::split(int max_size){
	max_size /= 2;
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

void Message::setRPCId(int c)
{
	rpc_id = c;
}
void Message::print(){
	cout<<"\nType: "<<message_type<<endl;
	printf("RPC Id: %d\nparts number: %d\npart number:%d\noperation: %d\nmessage_size: %d\n",rpc_id,parts_num,part_num,operation,message_size);
	string temp;
	temp.resize(message_size);
	for(int i=0;i<message_size;i++)
		temp[i]=message[i];
	cout<<temp<<endl;
	cout<<"end print//////////////////////////////////////\n"<<endl;
}

Message::~Message(){
	delete []message;
}

