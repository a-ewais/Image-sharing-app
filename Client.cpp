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
	else{
		cout<<"message number: "<<temp->getRPCId()<<" sent successfully and received reply\n";
//		temp->print();
	}
	return temp;
}
string Client::signup(string username, string password, sockaddr_in addr){
	Message r(Request);
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(password);
	args[2].setString(MessageDecoder::encodeIpPort(addr));
	MessageDecoder::encode(r, args, 1, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getString();
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
	delete reply;
	return args[0].getString();
}
map<string, string> Client::requestListOfOnlineUsers(string token, string username){
	Message r(Request);
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 6, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getMapSS();
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
	//TODO: add ip port field in up
	MessageDecoder::encode(r, args, 3, Request);
	Message* reply = execute(&r);
	if(reply==NULL)
		return false;
	delete reply;
	return true;
}
bool Client::down(string username, string token){
	Message r(Request);
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 4, Request);
	Message* reply = execute(&r);
	if(reply==NULL)
		return false;
	delete reply;
	return true;
}
vector<string> Client::requestListOfImages(string username, string token){
	Message r(Request);
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 7, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getVectorString();
}
string Client::requestImage(string username, string token, string imageId){
	Message r(Request);
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(token);
	args[2].setString(imageId);
	MessageDecoder::encode(r, args, 8, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getString();
}
bool Client::updateViews(string username, string token,  string imgId, int views){
	Message r(Request);
	vector<Parameter> args(4);
	args[0].setString(username);
	args[1].setString(token);
	args[2].setString(imgId);
	args[3].setInt(views);
	MessageDecoder::encode(r, args, 9, Request);
	Message* reply = execute(&r);
	if(reply==NULL)
		return false;
	delete reply;
	return true;
}

bool Client::revokeView(string username, string token, string imgId){
	if(!updateViews(username, token, imgId, 0))
	{
		Message r(Request);
		vector<Parameter> args(4);
		args[0].setString(username);
		args[1].setString(token);
		args[2].setString(peerUserName);
		args[3].setString(imgId);
		MessageDecoder::encode(r, args, 5, Request);
		Message* reply = execute(&r);
		if(reply==NULL)
			return false;
		delete reply;
		return true;
	}
	return true;
}

Client::~Client(){
	delete udpSocket;
}


