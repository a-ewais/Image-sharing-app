//============================================================================
// Name        : Distributed_Project.cpp
// Author      : Ahmed Ewais
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Server.h"
#include "Client.h"
#include "UDPServerSocket.h"
#include <cstring>
#include<string>
#include<cstdio>
#include <fstream>
#include <algorithm>
#include <streambuf>
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
//	srand ( unsigned ( std::time(0) ) );
	if(argv[1][0] == '0'){
		UDPServerSocket serv ("localhost", 64000);
		int duration = 10;
		while(true){
			if(serv.readyRequest())
			{
				Message* temp = serv.getRequest();
				temp->print();
				std::ifstream t("Text.txt");
				std::string all((std::istreambuf_iterator<char>(t)),
									std::istreambuf_iterator<char>());
				cout<<all<<endl;
				Message* m= new Message(Reply);
				m->setOperation(temp->getOperation());
				m->setRPCId(temp->getRPCId());
				m->setMessage(all.c_str(), all.size());
				m->print();
				serv.sendReply(m);
			}else{
				sleep(2);
			}
		}
	}
	else{
		Client cli("localhost", 64000);
		while(1){
			std::string _temp;
			std::cout << "Please Enter a message: ";
			getline(std::cin,_temp);
			Message* request = new Message(Request);
			request->setMessage(_temp.c_str(),_temp.size());
			Message* response = cli.execute(request);
			response->print();
		}
	}

//	std::ifstream t("Text.txt");
//	std::string all((std::istreambuf_iterator<char>(t)),
//					std::istreambuf_iterator<char>());
//	cout<<all.size()<<endl;
//	cout<<sizeof(all)<<endl;
////	cout<<all<<endl;
//
//	Message* m= new Message(Reply);
//	m->setMessage(all.c_str(), all.size());
//	int temp = 0;
//
////	m->print();
//
//	vector<Message*> l = m->split(7*1024);
//	random_shuffle(l.begin(),l.end());
//	cout<<l.size()<<endl;
//	vector<Message*> after;
//	for(int i=0;i<l.size();i++){
//		temp = 0;
//		after.push_back(new Message(l[i]->marshal(temp)));
//		delete l[i];
//	}
//
//	Message* mm = new Message(after);
//	mm->print();
//	string aft(mm->getMessage());
//	cout<<aft.size()<<endl;
//	cout<<sizeof(aft)<<endl;
////	cout<<aft<<endl;
//	cout<<(all==aft)<<endl;



	return 0;
}
