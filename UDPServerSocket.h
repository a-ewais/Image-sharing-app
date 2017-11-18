/*
 * UDPServerSocket.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */

#include "UDPSocket.h"

#ifndef UDPSERVERSOCKET_H_
#define UDPSERVERSOCKET_H_

class UDPServerSocket : public UDPSocket
{
	bool initializeServer (char * _myAddr, int _myPort);
public:
 UDPServerSocket (char * _myAddr, int _myPort);
 bool readyRequest();  //returns true if there is a ready request
 Message* getRequest();	//returns a message if there is a ready request, Null otherwise
 void sendReply(Message* m);	//sends the reply to the request sender
 bool sendReplyWaitAck(Message* m, int waitSec);		//sends the reply and waits for ack for waitSec
~UDPServerSocket ( );
};

#endif /* UDPSERVERSOCKET_H_ */
