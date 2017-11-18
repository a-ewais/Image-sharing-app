/*
 * UDPClientSocket.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: ewais
 */
#include "UDPClientSocket.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <time.h>
using namespace std;

extern "C" {
char * inet_ntoa(struct in_addr);
}

void UDPClientSocket::messenger(UDPClientSocket* me){

	int WORK;
	while(true){
		WORK = 0;
		if(me->readyToRead())
		{
			WORK++;
			char* incoming = new char[MAX_DATAGRAM_SIZE];
			int trials = 2;
			while(trials--){
				int res = me->readFromSocketWithBlock(incoming, MAX_DATAGRAM_SIZE);
				if(res<-1)
					cout<<"Error reading packet..\n";
				else
					break;
			}
			Message* temp = new Message(incoming);
			pthread_mutex_lock(&me->in_mutex);
			if(me->waitFor.find(temp->getRPCId())!=me->waitFor.end()){
				if(temp->isComplete()){
					me->waitFor[temp->getRPCId()] = temp;
					pthread_cond_signal(&me->cond);
					pthread_mutex_unlock(&me->in_mutex);
				}else{
					me->parts[temp->getRPCId()].push_back(temp);
					if(me->parts[temp->getRPCId()].size()==temp->getPartsNum()){
						temp = new Message(me->parts[temp->getRPCId()]);
						pthread_mutex_lock(&me->in_mutex);
						me->waitFor[temp->getRPCId()] = temp;
						me->waitFor.erase(temp->getRPCId());
						pthread_cond_signal(&me->cond);
						pthread_mutex_unlock(&me->in_mutex);
					}
				}
			}else
				delete [] temp;
		}
		pthread_mutex_lock(&me->out_mutex);
		if(!me->outbox.empty())
		{
			WORK++;
			Message* temp = me->outbox.front();
			me->outbox.pop();
			pthread_mutex_unlock(&me->out_mutex);
			int trials = 3;
			while(trials--){
				int t = 0;
				char* x = temp->marshal(t);
				int res = me->writeToSocket(x,t);
				if(res<0)
					cout<<"error sending packet\n";
			}
		}
		else
			pthread_mutex_unlock(&me->out_mutex);
		if(!WORK)
			sleep(3);
	}
}
UDPClientSocket::UDPClientSocket(char * _peerAddr, int _peerPort):UDPSocket(){
	in_mutex = PTHREAD_MUTEX_INITIALIZER;
	out_mutex = PTHREAD_MUTEX_INITIALIZER;
	cond = PTHREAD_COND_INITIALIZER;
	if(!initializeClient(_peerAddr, _peerPort))
		cout<<"failed to initialize Socket\n";

}
bool UDPClientSocket::initializeClient (char * _peerAddr, int _peerPort){
	lock();
	//initialize server socket address using the address and port
	peerAddress = new char[strlen(_peerAddr)+1];
	strcpy(peerAddress,_peerAddr);
	peerPort = _peerPort;
	struct hostent *host;
	peerAddr.sin_family = AF_INET;
	if((host = gethostbyname(peerAddress))== NULL){
		perror("Unknown host name\n");
		unlock();
		return false;
	}
	peerAddr.sin_addr = *(struct in_addr *) (host->h_addr);
	peerAddr.sin_port = htons(peerPort);
	//build my socket address
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(0);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddress = inet_ntoa(myAddr.sin_addr);
	myPort = ntohs(myAddr.sin_port);
	//setting timeout
//	//TODO: consider using timeout using poll instead of this.
//	struct timeval tv;
//	tv.tv_sec = 15;  /* 30 Secs Timeout */
//	tv.tv_usec = 0;  // Not init'ing this can cause strange errors
//	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
	//end timeout
	if(bind(sock, (const sockaddr*)(&myAddr), sizeof(struct sockaddr_in))!= 0){
		perror("Bind failed\n");
		close(sock);
		unlock();
		return false;
	}
	unlock();
	return true;
 }

void UDPClientSocket::send(Message* m){			//no delivery guarantees
	vector<Message*> temp_parts;
	if(m->getMessageSize()+32 > MAX_DATAGRAM_SIZE)
		temp_parts = m->split(MAX_DATAGRAM_SIZE);
	else
		temp_parts.push_back(m);
	pthread_mutex_lock(&out_mutex);
	for(int i=0;i<temp_parts.size();i++)
		outbox.push(temp_parts[i]);
	pthread_mutex_unlock(&out_mutex);
}


Message* UDPClientSocket::sendWaitForReply(Message* m, int waitSec){		//send request..wait for reply
	pthread_mutex_lock(&in_mutex);
	waitFor[m->getRPCId()] = NULL;
	pthread_mutex_unlock(&in_mutex);
	send(m);
	struct timespec time_to_wait = {0, 0};
	time_to_wait.tv_sec = time(NULL) + waitSec;
	pthread_mutex_lock(&in_mutex);
	pthread_cond_timedwait(&cond, &in_mutex, &time_to_wait);
	Message* temp = waitFor[m->getRPCId()];
	waitFor.erase(m->getRPCId());
//	parts.erase(m->getRPCId());
	pthread_mutex_unlock(&in_mutex);
	return temp;
}

UDPClientSocket::~UDPClientSocket (){
	pthread_mutex_destroy(&in_mutex);
	pthread_mutex_destroy(&out_mutex);
	pthread_cond_destroy(&cond);
}


