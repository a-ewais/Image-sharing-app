/*
 * Client.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include "UDPClientSocket.h"

#ifndef CLIENT_H_
#define CLIENT_H_

class Client
{
private:
	UDPClientSocket * udpSocket;
public:
	Client(char * _hostname, int _port);
	Message* execute(Message * _message);
	~Client();
};

#endif /* CLIENT_H_ */
