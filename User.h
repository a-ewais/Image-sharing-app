/*
 * User.h
 *
 *  Created on: Nov 21, 2017
 *      Author: ewais
 */

#ifndef USER_H_
#define USER_H_
#include "Client.h"
#include "ServiceDiscovery.h"
#include "ServerPeer.h"

struct Image;  //should contain the image and all its info..who owns it..who can see..number of views..

class User {
private:
	string username;
	string token;
	ServerPeer* myServer;
	map<string, Client*> clients;
	map<string, string> listOfUsers;

public:
	User(string serverIp, int serverPort);
	//from the service discovery
	bool signUp(string userName, string password);
	bool signIn(string userName, string password);
	//from other peers
	vector<string> viewOnlineUsers();	//who is online
	vector<string> getListOfPeerImages(string username);	//bayna y3ny
	int requestPeerImage(string username, string imagename);  //returns number of views if granted..negative otherwise
	Image* viewPeerImage(string username, string imagename);	//if it is granted...ask it from the server
	//from himself
	vector<string> viewMyImages(); //returns list of my images
	Image* viewMyImage(string imagename); //returns the image
	void updateMyImage(Image newImage); //adds new viewers..update counts
	//to other peers
	void grantPeerImage(string username, string imagename, int views);
	void revokePeerImage(string username, string imagename);
	void updatePeerImage(string username, string imagename, int views);
	~User();
};

#endif /* USER_H_ */
