/*
 * Client.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include "Message.h"
#include "UDPClientSocket.h"

#ifndef CLIENT_H_
#define CLIENT_H_

class Client
{
private:
	UDPClientSocket * udpSocket;
public:
	Client(char * _hostname, int _port);
	char* execute(char * _Message, int _size);
	Message* execute(Message * _message);
	~Client();
};

#endif /* CLIENT_H_ */
