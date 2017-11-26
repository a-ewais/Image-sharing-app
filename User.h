#ifndef USER_H_
#define USER_H_
#include "Client.h"
#include "ServiceDiscovery.h"
#include "ServerPeer.h"

struct Image{
	;  //should contain the image and all its info..who owns it..who can see..number of views..
};

class User {
private:
	string username;
	string token;
	ServerPeer* myServer;
	map<string, Client*> clients;
	map<string, string> listOfUsers;
	map<string, vector<string>> imageLists;

public:
	User(string serverIp, int serverPort,string myIp, int myPort);

	bool signUp(string userName, string password);
	bool signIn(string userName, string password);

	vector<string> viewOnlineUsers();
	vector<string> getListOfPeerImages(string username);
	void requestPeerImage(string username, string imagename, int views);
	cv::Mat viewPeerImage(string username, string imagename);

	vector<string> viewMyImages();
	cv::Mat viewMyImage(string imagename);
	void updateMyImage(cv::Mat newImage);
	void uploadImage(string imgPath);

	//to other peers
	void grantPeerImage(string username, string imagename, int views);
	void revokePeerImage(string username, string imagename);
	void updatePeerImage(string username, string imagename, int views);
	~User();
};

#endif /* USER_H_ */
