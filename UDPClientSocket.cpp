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

extern "C" {
char * inet_ntoa(struct in_addr);
}

UDPClientSocket::UDPClientSocket():UDPSocket(){

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
UDPClientSocket::~UDPClientSocket (){
}


