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
	queue<Message *> inbox, outbox, acks;
	pthread_mutex_t in_mutex, out_mutex, ack_mutex;
	bool initializeServer (char * _myAddr, int _myPort);
	static void* messenger(void* arg);

public:
	UDPServerSocket (char * _myAddr, int _myPort);
	bool readyRequest();  //returns true if there is a ready request
	Message* getRequest();	//returns a message if there is a ready request, Null otherwise
	void sendReply(Message* m);	//sends the reply to the request sender
	bool sendReplyWaitAck(Message* m, int trials);		//sends the reply and waits for ack for waitSec
	sockaddr_in getMyAddr();
	pair<string, int> getMyAddrPair();
	~UDPServerSocket ();
};

#endif /* UDPSERVERSOCKET_H_ */
