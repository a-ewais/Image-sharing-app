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
#include <iostream>


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

//bool UDPSocket::initializeServer (char * _myAddr, int _myPort){
//	lock();
//	myAddress = new char[strlen(_myAddr)+1];
//	strcpy(myAddress,_myAddr);
//	myPort = _myPort;
//	myAddr.sin_family = AF_INET;
//	myAddr.sin_port = htons(myPort);
//	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	if(bind(sock, (const sockaddr*)(&myAddr), sizeof(struct sockaddr_in))!= 0){
//		perror("Bind failed\n");
//		close(sock);
//		unlock();
//		return false;
//	}
//	unlock();
//	return true;
//}
//bool UDPSocket::initializeServer (char * _myAddr, int _myPort){
//	return false;
//}
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

UDPSocket::~UDPSocket(){
	printf("Destroying Socket\n");
	lock();
	close(sock);
	delete []myAddress;
	delete []peerAddress;
	unlock();
	pthread_mutex_destroy(&mutex);
	printf("socket destroyed\n");
}
