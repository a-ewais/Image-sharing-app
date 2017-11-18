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
#include <cstring>
#include<string>
#include<cstdio>
#include <fstream>
#include <streambuf>
using namespace std;

int main(int argc, char *argv[]){
//	if(argv[1][0] == '0'){
//		Server serv("localhost", 64000);
//		serv.serveRequest();
//	}
//	else{
//		Client cli("localhost", 64000);
//		while(1){
//			std::string _temp;
//			std::cout << "Please Enter a message: ";
//			getline(std::cin,_temp);
//			char* message = const_cast<char *>(_temp.c_str());;
//			char* response = cli.execute(message, strlen(message));
//			std::cout << response << endl;
//		}
//	}

	std::ifstream t("Text.txt");
	std::string all((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());
	cout<<all.size()<<endl;
	cout<<sizeof(all)<<endl;
//	cout<<all<<endl;

	Message* m= new Message(Reply);
	m->setMessage(all.c_str(), all.size());
	m->print();
	vector<Message*> l = m->split(7*1024);
	cout<<l.size()<<endl;

	Message* mm = new Message(l);
	string aft(mm->getMessage());
	cout<<aft.size()<<endl;
	cout<<sizeof(aft)<<endl;
//	cout<<aft<<endl;
	cout<<(all==aft)<<endl;



	return 0;
}
