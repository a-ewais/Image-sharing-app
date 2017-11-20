#ifndef SERVER_PEER_H
#define SERVER_PEER_H

#include "Server.h"
#include <vector>
#include <string>

class ServerPeer: protected Server{
    private:
		std::vector<std::string> imageList;

        std::vector<std::string> getListofImages(std::string token, std::string userID);
        std::string getImage(std::string token, std::string userID, std::string imageID);
        void updateViews(std::string token, std::string userID, int count);
        void revokeViews(std::string token, std::string userID);

   		Message* doOperation(Message* _message); //overload...dispatcher
    public:
        ServerPeer(char* _listen_hostname, int _listen_port);
        ~ServerPeer();
};

#endif
