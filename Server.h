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
	UDPServerSocket * udpServerSocket;
	Message * getRequest();
	Message * doOperation();
	void sendReply (Message * _message);
public:
	Server(char * _listen_hostname, int _listen_port);
	void serveRequest();
	~Server();
};



#endif /* SERVER_H_ */
