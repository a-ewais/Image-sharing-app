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
public:
 UDPServerSocket ();
bool initializeServer (char * _myAddr, int _myPort);
~UDPServerSocket ( );
};

#endif /* UDPSERVERSOCKET_H_ */
