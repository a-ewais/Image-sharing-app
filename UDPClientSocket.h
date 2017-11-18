/*
 *  UDPClientSocket.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include "UDPSocket.h"
#include "Message.h"
#include <queue>
#include <map>
#include <set>
using namespace std;

#ifndef UDPCLIENTSOCKET_H
#define UDPCLIENTSOCKET_H


class UDPClientSocket : public UDPSocket
{
private:
	queue<Message *> outbox;
	map<int, Message*> waitFor;
	map<int, vector<Message*> > parts;
	pthread_mutex_t out_mutex, in_mutex;
	pthread_cond_t cond;
	bool initializeClient (char * _peerAddr, int _peerPort);
	static void* messenger(void* me);
	void send(Message* m);

public:
	UDPClientSocket (char * _peerAddr, int _peerPort);
	Message* sendWaitForReply(Message* m, int sec);
	~UDPClientSocket ();
};


#endif /* UDPCLIENTSOCKET_H_ */
