/*
 * Client.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: ewais
 */

#include"Client.h"

Client::Client(char * _hostname, int _port){
	udpSocket = new UDPClientSocket();
	udpSocket->initializeClient(_hostname, _port);
}

Message * Client::execute(Message * _message){
	Message * reply = new
}


