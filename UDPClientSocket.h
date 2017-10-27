/*
 *  UDPClientSocket.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */
#include "UDPSocket.h"

#ifndef UDPCLIENTSOCKET_H
#define UDPCLIENTSOCKET_H


class UDPClientSocket : public UDPSocket
{
public:
 UDPClientSocket ();
 bool initializeClient (char * _peerAddr, int _peerPort);
~UDPClientSocket ();
};


#endif /* UDPCLIENTSOCKET_H_ */
