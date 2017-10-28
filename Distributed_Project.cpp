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
		Server serv("localhost", 16333);
		serv.serveRequest();
	}
	else{
		Client cli("localhost", 16333);
		while(1){
			char* message;
			std::cout << "Please Enter a message: ";
			char* c;
			scanf("%1024s[^\n]%c", message, c);
			char* response = cli.execute(message, strlen(message));
			std::cout << response << endl;
		}
	}

	return 0;
}
