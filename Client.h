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
	string login(string username, string password, sockaddr_in addr);
	map<string, string> requestListOfOnlineUsers(string token, string username);
	bool auth(string username, string token);
	bool up(string username, string token, sockaddr_in addr);
	void down(string username, string token);
	vector<string> requestListOfImages(string username, string token, sockaddr_in peer);
	string requestImage(string username, string token, sockaddr_in peer, string imageId);
	bool updateViews(string username, string token, sockaddr_in peer, string imgId, int views);
	bool revokeView(string username, string token, sockaddr_in peer, string imgId);
	bool revokeViewFailed(string username, string token, string userRevoked, string imgId);
	~Client();
};

#endif /* CLIENT_H_ */
