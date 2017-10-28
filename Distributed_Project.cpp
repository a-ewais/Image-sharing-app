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

using namespace std;

int main(int argc, char *argv[]){
	if(argv[1][0] == '0'){
		Server serv("localhost", 64000);
		serv.serveRequest();
	}
	else{
		Client cli("localhost", 64000);
		while(1){
			std::string _temp;
			std::cout << "Please Enter a message: ";
			getline(std::cin,_temp);
			char* message = const_cast<char *>(_temp.c_str());;
			char* response = cli.execute(message, strlen(message));
			std::cout << response << endl;
		}
	}

	return 0;
}
