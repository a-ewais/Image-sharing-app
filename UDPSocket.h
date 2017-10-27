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
public:
	UDPSocket ();	//Done
	void setFilterAddress (char * _filterAddress); //what is it?
	char * getFilterAddress ();		//what is that?
	//bool initializeServer (char * _myAddr, int _myPort); //Done
	//bool initializeClient (char * _peerAddr, int _peerPort); //Done
	int writeToSocket (char * buffer, int maxBytes );
	int writeToSocketAndWait (char * buffer, int maxBytes,int microSec );
	int readFromSocketWithNoBlock (char * buffer, int maxBytes );
	int readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec,
	int timeoutMilli);
	int readFromSocketWithBlock (char * buffer, int maxBytes );
	int readSocketWithNoBlock (char * buffer, int maxBytes );
	int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int
	timeoutMilli);
	int readSocketWithBlock (char * buffer, int maxBytes );
	int getMyPort (); //Done
	int getPeerPort (); //Done
	void enable();	//Done
	void disable(); //Done
	bool isEnabled();	//Done
	void lock();	//Done
	void unlock();	//Done
	int getSocketHandler(); //will cause Problem...leave it for now
	~UDPSocket ( ); //Done
};

#endif /* UDPSOCKET_H_ */
