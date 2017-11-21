/*
 * UDPServerSocket.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */

#include "UDPSocket.h"
#include "Message.h"
#include <map>
#include <queue>
using namespace std;
#ifndef UDPSERVERSOCKET_H_
#define UDPSERVERSOCKET_H_

class UDPServerSocket:private UDPSocket
{
private:
	map <int, pair<int, string> > id_ip;
	map <pair<int, string>, int> ip_id;
	map <int, vector<Message*> > parts;
	queue<Message *> inbox, outbox;
	pthread_mutex_t in_mutex, out_mutex;
	bool initializeServer (char * _myAddr, int _myPort);
	static void* messenger(void* arg);

public:
	UDPServerSocket (char * _myAddr, int _myPort);
	bool readyRequest();  //returns true if there is a ready request
	Message* getRequest();	//returns a message if there is a ready request, Null otherwise
	void sendReply(Message* m);	//sends the reply to the request sender
	bool sendReplyWaitAck(Message* m, int waitSec);		//sends the reply and waits for ack for waitSec
	sockaddr_in getMyAddr();
	~UDPServerSocket ();
};

#endif /* UDPSERVERSOCKET_H_ */
