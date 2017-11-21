/*
 * UDPSocket.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include <pthread.h>
#include <netinet/in.h>
#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#define MAX_DATAGRAM_SIZE 7*1024   //8 KB

class UDPSocket
{
protected:
	int sock;
	sockaddr_in myAddr;
	sockaddr_in peerAddr;
	char * myAddress;
	char * peerAddress;
	int myPort;
	int peerPort;
	bool enabled;
	pthread_mutex_t mutex;
	UDPSocket ();	//Done
	void setFilterAddress (char * _filterAddress); //what is it?
	char * getFilterAddress ();		//what is that?
	//bool initializeServer (char * _myAddr, int _myPort); //Done
	//bool initializeClient (char * _peerAddr, int _peerPort); //Done
	int writeToSocket (char * buffer, int maxBytes ); //Done
	int writeToSocketAndWait (char * buffer, int maxBytes,int milliSec ); //wait for what??
	int readFromSocketWithNoBlock (char * buffer, int maxBytes );
	int readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec); //Done
	int readFromSocketWithBlock (char * buffer, int maxBytes ); //Done
	bool readyToRead();
//	int readSocketWithNoBlock (char * buffer, int maxBytes );
//	int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int
//	timeoutMilli);
	void setPeer(sockaddr_in x);
	void enable();	//Done
	void disable(); //Done
	bool isEnabled();	//Done
	void lock();	//Done
	void unlock();	//Done
	int getSocketHandler(); //Done
	int getMyPort (); //Done
	int getPeerPort (); //Done

	virtual ~UDPSocket ( ); //Done
};

#endif /* UDPSOCKET_H_ */
