#include <opencv2/core/core.hpp>
#include "User.h"

User::User(string serverIp, int serverPort, string myIp, int myPort) {
	clients["ServiceDiscovery"] = new Client(const_cast<char*>(serverIp.c_str()), serverPort, "ServiceDiscovery");
	myServer = new ServerPeer(const_cast<char*>(myIp.c_str()), myPort, clients["ServiceDiscovery"]);
}

bool User::signUp(string _userName, string _password){
	Client* cli = clients["ServiceDiscovery"];
	pair<string, int> temp = myServer->getMyAddrPair();
	string _token = cli->signup(_userName, _password, temp.first, temp.second);

	if(_token=="")
		return false;

	token = _token;
	username = _userName;

	return true;
}

bool User::signIn(string _userName, string _password){
	Client* cli = clients["ServiceDiscovery"];
	pair<string, int> temp = myServer->getMyAddrPair();
	string _token = cli->login(_userName, _password, temp.first,
			temp.second);

	if(_token == "")
		return false;

	token = _token;
	username = _userName;
	return true;
}

vector<string> User::viewOnlineUsers(){
	Client* cli = clients["ServiceDiscovery"];
	cout<<"list of ";
	listOfUsers = cli->requestListOfOnlineUsers(token, username);
	vector<string> _onlineUsers;

	for(auto it=listOfUsers.begin();it!=listOfUsers.end();it++)
		_onlineUsers.push_back(it->first);

	return _onlineUsers;
}

vector<string> User::getListOfPeerImages(string _username){
	if(clients.find(_username)==clients.end()){
		cout<<"new client "<< _username;
		pair<string,int> temp = MessageDecoder::decodeIpPortPair(listOfUsers[_username]);
		clients[_username] = new Client(const_cast<char*>(temp.first.c_str()), temp.second, _username);
	}

	Client* cli = clients[_username];
//	if(!cli->isEnabled())
//		cli->enable();

	vector<string> _listofImages = cli->requestListOfImages(username, token);
	imageLists[_username] = _listofImages;
//	cli->diable();
	return _listofImages;
}

void User::requestPeerImage(string _username, string _imagename, int views){
	Client* cli = clients[_username];
//	if(!cli->isEnabled())
//		cli->enable();
	bool request = cli->requestImage(username, token, _imagename, views);
//	myServer->writePeerImage(_username, _imagename, image);
}

cv::Mat User::viewPeerImage(string _username, string _imagename){
	cv::Mat m = myServer->readPeerImage(_username, _imagename);
	//TODO: MODIFY REMAINING VIEWS
//	clients[_username]->viewNotify(username, token, _imagename);
	return m;
}

vector<string> User::viewMyImages(){
	return myServer->getListOfMyImages();
}

cv::Mat User::viewMyImage(string imagename){
	return myServer->getMyImage(imagename);
}

void User::updateMyImage(cv::Mat newImage){
	//TODO: can be deleted
}

void User::uploadImage(string imgPath){
	myServer->writeMyImage(imgPath);
}

void User::grantPeerImage(string _username, string _imagename, int _views){
	Client* cli = clients[_username];
	myServer->updateLocalViews(_username, _imagename, _views);
	string image = myServer->getImage(_imagename);
	bool sent = cli->sendImage(username, token, _imagename, image);
	if(sent);
		//TODO:Remove Requester from server
}

void User::revokePeerImage(string _username, string _imagename){
	Client* cli = clients[_username];
//	if(!cli->isEnabled())
//		cli->enable();
	while(!cli->revokeView(username,token,_imagename));
//	cli->disable();
	myServer->updateLocalViews(_username, _imagename, 0);
}

void User::updatePeerImage(string _username, string _imagename, int _views){
	Client* cli = clients[_username];
	//	if(!cli->isEnabled())
	//		cli->enable();
	if(cli->updateViews(username, token, _imagename, _views))
		myServer->updateLocalViews(_username, _imagename, _views);
	//	cli->disable();
}

User::~User() {
	for(auto it= clients.begin();it!=clients.end();it++)
		delete it->second;
	delete myServer;
}

