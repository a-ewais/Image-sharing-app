#include "MessageDecoder.h"
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>

using namespace std; 

MessageDecoder::MessageDecoder(){
}

MessageDecoder::~MessageDecoder(){
}

void MessageDecoder::encode(Message& _message, std::vector<Parameter>& params, int& _operation, MessageType& _type){
    
    string messageContent;
    _message.setOperation(_operation);
    _message.setMessageType(Request);

    switch(_operation){
        case 1:
            messageContent.append(params[0].getString());
            messageContent.append(";");
            messageContent.append(params[1].getString());
            _message.setMessage(messageContent.c_str(), messageContent.size());
        break;
        case 2:
            //Extract Message
            // string token = signIn(username,password,ip); 
            //Send Reply 
        break;
        case 3:

            //Extract Message
        //    stillUp(username,token); 
            //Send Reply 
        break;
        case 4:
            // string username,token;
            //Extract Message
            // stillUp(username,token);
            //Send Reply 
        break;
        case 5:
            //Extract Message
            //pendingRequest()
            //Send Reply 
        break;
        case 6:
            // string username,token;
            //Extract Message
            // getListOfOnlineUsers(username,token);
            //Send Reply 
        break;
        case 7:
            // string  token,userID,imageID;
            //Extract Message
            // string image = getImage(token,userID,imageID);
            //Send Reply 
        break;
        case 8:
            //Extract Message
            //Call Function 1
            //Send Reply 
            break;
        default:
            //Extract Message
            //Call Function 1
            //Send Reply 
        break;
    }

}

void MessageDecoder::decode(Message& _message, std::vector<Parameter>& params, int& _operation, MessageType& _type){
	cout << "Started Decode!" << endl;
    _operation = _message.getOperation();
    _type = _message.getMessageType();
	cout << "Checkpoint 1!" << endl;
	std::vector<std::string> tokens;

    char* pch = strtok (_message.getMessage(),";");
    while (pch != NULL)
    {
    	tokens.push_back(pch);
    	printf ("%s\n",pch);
    	pch = strtok (NULL, " ,.-");
    }

    switch(_operation){
        case 1:
            Parameter username, password;
            username.setString(tokens[0]);
            cout << tokens[0] << endl;
            password.setString(tokens[1]);
            params.push_back(username);
            params.push_back(password);
            break;
    }

}
