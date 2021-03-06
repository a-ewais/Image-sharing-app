#include "UDPClientSocket.h"
#include <opencv2/opencv.hpp>

#ifndef CLIENT_H_
#define CLIENT_H_

class Client
{
private:
	string peerUserName;
	UDPClientSocket * udpSocket;
public:
	Client(char * _hostname, int _port, string peerUserName);
	Message* execute(Message * _message);
	string signup(string& username, string& password, string& hostname, int port);
	string login(string& username, string& password, string& hostname, int port);
	map<string, string> requestListOfOnlineUsers(string& token, string& username);
	bool auth(string& username, string& token);
	bool up(string& username, string& token, string& hostname, int port);
	bool down(string& username, string& token);
	vector<string> requestListOfImages(string& username, string& token);
	bool requestImage(string& username, string& token, string& imageId, int& views);
	bool sendImageApprove(string& username, string& token,string& imageID, int& views, bool& approve);
	bool updateViews(string& username, string& token, string& imgId, int views);
	bool revokeView(string& username, string& token, string& imgId);
	void viewNotify(string& username, string& token,string& imgId);
	string getApprovedImage(string& username, string& token, string& imageID);
	~Client();
};

#endif /* CLIENT_H_ */
