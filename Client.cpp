#include<iostream>
#include<vector>
#include"Client.h"
#include"MessageDecoder.h"
#include <opencv2/opencv.hpp>

using namespace std;

Client::Client(char* _hostname, int _port, string _peerUserName){
	udpSocket = new UDPClientSocket(_hostname, _port);
	peerUserName = _peerUserName;
}

Message * Client::execute(Message* message){
	message->print();
	Message* temp = udpSocket->sendWaitForReply(message, 30);
	if(temp==NULL)
		cout<<"failed to send message..network error\n";
	else{
		cout<<"message number: "<<temp->getRPCId()<<" sent successfully and received reply\n";
		temp->print();
	}
	return temp;
}

string Client::signup(string& username, string& password, string& host, int port){
	Message r(Request, Message::getNewRPC());
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(password);
	args[2].setString(MessageDecoder::encodeIpPortPair(host, port));
	MessageDecoder::encode(r, args, 1, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getString();
}

string Client::login(string& username, string& password, string& host, int port){
	Message r(Request, Message::getNewRPC());
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(password);
	args[2].setString(MessageDecoder::encodeIpPortPair(host, port));
	MessageDecoder::encode(r, args, 2, Request);
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
//	reply->print();
	delete reply;
	return args[0].getString();
}

map<string, string> Client::requestListOfOnlineUsers(string& token, string& username){
	Message r(Request, Message::getNewRPC());
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

bool Client::auth(string& username, string& token){
	Message r(Request, Message::getNewRPC());
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

bool Client::up(string& username, string& token, string& host, int port){
	Message r(Request, Message::getNewRPC());
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(token);
	args[2].setString(MessageDecoder::encodeIpPortPair(host, port));
	MessageDecoder::encode(r, args, 3, Request);
	Message* reply = execute(&r);
	if(reply==NULL)
		return false;
	delete reply;
	return true;
}

bool Client::down(string& username, string& token){
	Message r(Request, Message::getNewRPC());
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

vector<string> Client::requestListOfImages(string& username, string& token){
	Message r(Request, Message::getNewRPC());
	vector<Parameter> args(2);
	args[0].setString(username);
	args[1].setString(token);
	MessageDecoder::encode(r, args, 7, Request);
	r.print();
	Message* reply = execute(&r);
	args.clear();
	MessageDecoder::decode(reply, args);
	delete reply;
	return args[0].getVectorString();
}

bool Client::requestImage(string& username, string& token, string& imageId, int& views){
	Message r(Request, Message::getNewRPC());
	vector<Parameter> args(3);
	args[0].setString(username);
	args[1].setString(token);
	args[2].setString(imageId);
	args[3].setInt(views);
	MessageDecoder::encode(r, args, 8, Request);
	Message* reply = execute(&r);
	if(reply==NULL)
		return false;
	delete reply;
	return true;
//	args.clear();
//	MessageDecoder::decode(reply, args);
//	cout<<reply->getMessageSize()<<endl;
//	delete reply;
//	string imageData = args[0].getString();
//	cout << imageData.size() << endl;
//	return imageData;
}

bool Client::updateViews(string& username, string& token,  string& imgId, int views){
	Message r(Request, Message::getNewRPC());
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

bool Client::revokeView(string& username, string& token, string& imgId){
	if(!updateViews(username, token, imgId, 0))
	{
		Message r(Request, Message::getNewRPC());
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


