#include "UDPServerSocket.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "MessageDecoder.h"

extern "C" {
	char * inet_ntoa(struct in_addr);
}

UDPServerSocket::UDPServerSocket(char * _myAddr, int _myPort):UDPSocket(){
	in_mutex = PTHREAD_MUTEX_INITIALIZER;
	out_mutex = PTHREAD_MUTEX_INITIALIZER;

	if(!initializeServer(_myAddr, _myPort))
		cout<<"failed to initialize Socket\n";
	pthread_t p;
	pthread_create( &p, NULL, &UDPServerSocket::messenger, this);
}

bool UDPServerSocket::initializeServer (char * _myAddr, int _myPort){
	lock();
	myAddress = new char[strlen(_myAddr)+1];
	strcpy(myAddress,_myAddr);
	myPort = _myPort;
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(myPort);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock, (const sockaddr*)(&myAddr), sizeof(struct sockaddr_in))!= 0){
		perror("Bind failed\n");
		close(sock);
		unlock();
		return false;
	}
	peerAddr.sin_family = AF_INET;
	size_t nn = 1024*1024*10;
	if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &nn, sizeof(nn)) == -1) {
			cout<<"failed to resize buffer\n";
		}
	unlock();
	return true;
}

void* UDPServerSocket::messenger(void* arg){
	UDPServerSocket* me = (UDPServerSocket*)arg;
	int WORK;
	while(true){
		WORK = 0;
		if(me->readyToRead()){
			cout<<"inbox is not empty\n";
			WORK++;
			char* incoming = new char[MAX_DATAGRAM_SIZE];
			int trials = 2;
			while(trials--){
				int res = me->readFromSocketWithBlock(incoming, MAX_DATAGRAM_SIZE);
				if(res<0)
					cout<<"Error reading packet..\n";
				else
					break;
			}
			Message* temp = new Message(incoming);
			delete [] incoming;
			if(me->ip_id.find({temp->getRPCId(), MessageDecoder::encodeIpPort(me->peerAddr)})==me->ip_id.end()){
				int new_rpc = Message::getNewRPC();
				cout<<"the new local rpcid is: "<<new_rpc<<endl;
				me->ip_id[{temp->getRPCId(), MessageDecoder::encodeIpPort(me->peerAddr)}] = new_rpc;
				me->id_ip[new_rpc] = {temp->getRPCId(), MessageDecoder::encodeIpPort(me->peerAddr)};
				temp->setRPCId(new_rpc);
			}else{
				int new_rpc = me->ip_id[{temp->getRPCId(), MessageDecoder::encodeIpPort(me->peerAddr)}];
				temp->setRPCId(new_rpc);
			}

			if(temp->isComplete()){
				pthread_mutex_lock(&me->in_mutex);
				me->inbox.push(temp);
				pthread_mutex_unlock(&me->in_mutex);
			}else{
				me->parts[temp->getRPCId()].push_back(temp);
				if(me->parts[temp->getRPCId()].size()==temp->getPartsNum()){
					temp = new Message(me->parts[temp->getRPCId()]);
					pthread_mutex_lock(&me->in_mutex);
					me->inbox.push(temp);
					pthread_mutex_unlock(&me->in_mutex);
				}
			}

		}

		pthread_mutex_lock(&me->out_mutex);
		if(!me->outbox.empty())
		{
			cout<<"outbox is not empty";
			WORK++;
			Message* temp = me->outbox.front();
			me->outbox.pop();
			pthread_mutex_unlock(&me->out_mutex);
			int trials = 3;
			cout<<"this is the message to send\n";
			temp->print();
			int localRPCId = temp->getRPCId();
			temp->setRPCId(me->id_ip[localRPCId].first);

			while(trials--){
				int t = 0;
				char* x = temp->marshal(t);
				me->setPeer(MessageDecoder::decodeIpPort(me->id_ip[localRPCId].second));
				int res = me->writeToSocket(x,t);
				if(res<0)
					cout<<"error sending packet\n";
				else{
					cout<<"packet sent\n";
					break;
				}
			}
		}
		else
			pthread_mutex_unlock(&me->out_mutex);
		if(!WORK){
//			sleep(1);
			;
		}
	}

}
void UDPServerSocket::sendReply(Message* m){
	vector<Message*> temp_parts;
	if(m->getMessageSize()+32 > MAX_DATAGRAM_SIZE)
		temp_parts = m->split(MAX_DATAGRAM_SIZE);
	else
		temp_parts.push_back(m);
	cout<<"this message is split into "<<temp_parts.size()<<" parts\n";
	pthread_mutex_lock(&out_mutex);
	for(int i=0;i<temp_parts.size();i++){
		usleep(1000);
		outbox.push(temp_parts[i]);
	}
	pthread_mutex_unlock(&out_mutex);
}

bool UDPServerSocket::readyRequest(){
	bool flag = true;
	pthread_mutex_lock(&in_mutex);
	if(inbox.empty())
		flag = false;
	pthread_mutex_unlock(&in_mutex);
	return flag;
}

Message* UDPServerSocket::getRequest(){
	if(!readyRequest())
		return NULL;
	pthread_mutex_lock(&in_mutex);
	Message * temp = inbox.front();
	inbox.pop();
	pthread_mutex_unlock(&in_mutex);
	return temp;
}
sockaddr_in UDPServerSocket::getMyAddr(){
	return this->myAddr;
}

pair<string, int> UDPServerSocket::getMyAddrPair(){
	return {string(myAddress), myPort};
}
UDPServerSocket::~UDPServerSocket (){
	pthread_mutex_destroy(&in_mutex);
	pthread_mutex_destroy(&out_mutex);

};



