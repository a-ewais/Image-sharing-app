/*
 * User.cpp
 *
 *  Created on: Nov 21, 2017
 *      Author: ewais
 */

#include "User.h"

User::User(string serverIp, int serverPort) {
	clients["ServiceDiscovery"] = new Client(const_cast<char*>(serverIp.c_str()), serverPort, "ServiceDiscovery");
	myServer = new ServerPeer(const_cast<char*>(serverIp.c_str()), serverPort, clients["ServiceDiscovery"]);
}

bool User::signUp(string _userName, string _password){
	Client* cli = clients["ServiceDiscovery"];
	pair<string, int> temp = myServer->getMyAddrPair();
	string t = cli->signup(_userName, _password, temp.first,
			temp.second);
	if(t=="")
		return false;
	token = t;
	username = _userName;
	return true;
}

bool User::signIn(string _userName, string _password){
	Client* cli = clients["ServiceDiscovery"];
	pair<string, int> temp = myServer->getMyAddrPair();
	string t = cli->login(_userName, _password, temp.first,
			temp.second);
	if(t=="")
		return false;
	token = t;
	username = _userName;
	return true;
}

vector<string> User::viewOnlineUsers(){
	Client* cli = clients["ServiceDiscovery"];
	listOfUsers = cli->requestListOfOnlineUsers(token, username);
	vector<string> l;
	for(auto it=listOfUsers.begin();it!=listOfUsers.end();it++)
		l.push_back(it->first);
	return l;
}

vector<string> User::getListOfPeerImages(string _username){
	if(clients.find(_username)==clients.end()){
		pair<string,int> temp = MessageDecoder::decodeIpPortPair
				(listOfUsers[_username]);
		clients[_username] = new Client(const_cast<char*>(temp.first.c_str()), temp.second,
				_username);
	}
	Client* cli = clients[_username];
//	if(!cli->isEnabled())
//		cli->enable();
	//TODO:uncomment..maybe save the vector for later!?
	vector<string> v = cli->requestListOfImages(username, token);
//	cli->diable();
	return v;
}

int User::requestPeerImage(string _username, string _imagename){
	Client* cli = clients[_username];
//	if(!cli->isEnabled())
//		cli->enable();
	string v = cli->requestImage(username, token, _imagename);
	Image temp(v);
	myServer->writePeerImage(_username, _imagename, temp);
	int permViews;
//	permViews = temp.getViews();
//	cli->disable();
	//TODO: uncomment
	return permViews;
}

Image* User::viewPeerImage(string _username, string _imagename){
	Image* m;
	m = myServer->readPeerImage(_username, _imagename);
	clients[_username]->viewNotify(username, token, _imagename);
	return m;
}

vector<string> User::viewMyImages(){
	return myServer->getListOfMyImages();
}
Image* User::viewMyImage(string imagename){
	return myServer->getMyImage(imagename);
}
void User::updateMyImage(Image newImage){
	//TODO: can be deleted
}
void User::grantPeerImage(string _username, string _imagename, int _views){
	//TODO: when we have an idea how to notify the user and take his response.
	//we have to communicate the grant order to the server
}
void User::revokePeerImage(string _username, string _imagename){
	Client* cli = clients[_username];
//	if(!cli->isEnabled())
//		cli->enable();
	while(!cli->revokeView(username,token,_imagename));
//	cli->disable();
	//TODO: update the image locally
}
void User::updatePeerImage(string _username, string _imagename, int _views){
	Client* cli = clients[_username];
	//	if(!cli->isEnabled())
	//		cli->enable();
	cli->updateViews(username,token,_imagename,_views);
	//	cli->disable();
	//TODO: update the image locally
}
User::~User() {
	for(auto it= clients.begin();it!=clients.end();it++)
		delete it->second;
	delete myServer;
}

