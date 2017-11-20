/*
 * Client.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: ewais
 */
#include<iostream>
#include<vector>
#include"Client.h"
#include"MessageDecoder.h"
using namespace std;
Client::Client(char* _hostname, int _port){
	udpSocket = new UDPClientSocket(_hostname, _port);
}

Message * Client::execute(Message* message){

	Message* temp = udpSocket->sendWaitForReply(message, 30);
	if(temp==NULL)
		cout<<"failed to send message..network error\n";
	else
		cout<<"message number: "<<temp->getRPCId()<<" sent successfully and received reply\n";
	return temp;
}
string Client::login(string username, string password, sockaddr_in addr){
	Message r(Request);
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(password);
	args[2].setString(MessageDecoder::encodeIpPort(addr));
	MessageDecoder::encode(r, args, 2, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	//TODO: fix decode to be simpler
	delete reply;
	return args[0].getString();
}
map<string, string> Client::requestListOfOnlineUsers(string token, string username){
	//TODO: add support for map<string, string> in decoder encoder
	Message r(Request);
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 6, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getMapString();
}
bool Client::auth(string username, string token){
	Message r(Request);
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 10, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getBoolean();
}
bool Client::up(string username, string token, sockaddr_in addr){
	Message r(Request);
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(token);
	args[2].setString(MessageDecoder::encodeIpPort(addr));
	//TODO: add ip to up function in encoder decoder
	MessageDecoder::encode(r, args, 3, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	//TODO: make sure up returns boolean or think of other fix
	delete reply;
	return args[0].getBoolean();
}
bool Client::down(string username, string token){
	Message r(Request);
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 4, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	//TODO: make sure Down returns boolean or think of other fix
	delete reply;
	return args[0].getBoolean();
}
vector<string> Client::requestListOfImages(string username, string token){

}
string Client::requestImage(string username, string token, sockaddr_in peer, string imageId);
bool Client::updateViews(string username, string token, sockaddr_in peer, string imgId, int views);
bool Client::revokeView(string username, string token, sockaddr_in peer, string imgId);
bool Client::revokeViewFailed(string username, string token, string userRevoked, string imgId);
Client::~Client(){
	delete udpSocket;
}


