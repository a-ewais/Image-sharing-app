/*
 * UDPSocket.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include "UDPSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <poll.h>

UDPSocket::UDPSocket (){
	if((sock = socket(AF_INET, SOCK_DGRAM, 0))<0) {
		perror("socket failed");
		return;
	}
	myPort = -1;
	peerPort = -1;
	myAddress = NULL;
	peerAddress = NULL;
	pthread_mutex_init(&mutex, NULL);
	enabled = false;
	//TODO:setBroadcast(s)
}

int UDPSocket::writeToSocket (char * buffer, int maxBytes ){
	lock();
	int res = sendto(sock, buffer, maxBytes,
			0,(const sockaddr*)&peerAddr , sizeof(peerAddr));
	unlock();
	return res;
}

int UDPSocket::readFromSocketWithBlock (char * buffer, int maxBytes ){
	lock();
	unsigned int aLength = sizeof(peerAddr);
	int res = recvfrom(sock, buffer, maxBytes, 0, (sockaddr*)&peerAddr, &aLength);
	unlock();
	return res;
}

int UDPSocket::readFromSocketWithTimeout(char* buffer, int maxBytes, int timeoutSec){
	lock();
	struct pollfd fd;
	int res;
	fd.fd = sock;
	fd.events = POLLIN;
	res = poll(&fd, 1, timeoutSec*1000); // 1000 ms timeout
	if(res>0){
		unsigned int aLength = sizeof(peerAddr);
		int res = recvfrom(sock, buffer, maxBytes, 0, (sockaddr*)&peerAddr, &aLength);
	}
	unlock();
	return res;
}

bool UDPSocket::readyToRead(){
	lock();
	struct pollfd fd;
	int res;
	fd.fd = sock;
	fd.events = POLLIN;
	res = poll(&fd, 1, 0); // 1000 ms timeout
	unlock();
	return (res>0)? true: false;
}

int UDPSocket::getMyPort(){
	int res;
	lock();
	res = myPort;
	unlock();
	return res;
}
int UDPSocket::getPeerPort (){
	int res;
	lock();
	res = peerPort;
	unlock();
	return res;
}
void UDPSocket::setPeer(sockaddr_in x){
	peerAddr.sin_family = x.sin_family;
	peerAddr.sin_port = x.sin_port;
	peerAddr.sin_addr.s_addr = x.sin_addr.s_addr;
}
void UDPSocket::enable(){
	lock();
	if(enabled)
		printf("socket already enabled\n");
	else
		enabled = true;
	unlock();
}

void UDPSocket::disable(){
	lock();
	if(!enabled)
		printf("socket already disabled\n");
	else
		enabled = false;
	unlock();
}

bool UDPSocket::isEnabled(){
	bool res;
	lock();
	res = enabled;
	unlock();
	return res;
}
void UDPSocket::lock(){
	pthread_mutex_lock(&mutex);
}
void UDPSocket::unlock(){
	pthread_mutex_unlock(&mutex);
}
int UDPSocket::getSocketHandler(){
	return sock;
}
UDPSocket::~UDPSocket(){
	printf("Destroying Socket\n");
	lock();
	close(sock);
	unlock();
	pthread_mutex_destroy(&mutex);
	printf("socket destroyed\n");
}
