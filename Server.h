/*
 * Server.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include "Message.h"
#include "UDPServerSocket.h"

#ifndef SERVER_H_
#define SERVER_H_

class Server
{
private:
	UDPServerSocket* udpServerSocket;
	Message* getRequest();
	virtual Message* doOperation(Message* _message) = 0;
	void sendReply (Message* _message);
	bool sendReplyWithAck(Message* _message);
public:
	Server(char* _listen_hostname, int _listen_port);
	sockaddr_in getMyAddr();
	void startListen();
	virtual ~Server() = 0;
};



#endif /* SERVER_H_ */
