/*
 * UDPServerSocket.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: ewais
 */
#include "UDPServerSocket.h"
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

extern "C" {
char * inet_ntoa(struct in_addr);
}

UDPServerSocket::UDPServerSocket(char * _myAddr, int _myPort):UDPSocket(){

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
	unlock();
	return true;
}
UDPServerSocket::~UDPServerSocket (){};



