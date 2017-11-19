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
    _message.setMessageType(_type);

    switch(_operation){
        case 5:
        break;
        case 8:
            messageContent.append(params[0].getString());
            messageContent.append(";");
            messageContent.append(params[1].getString());
            messageContent.append(";");
            messageContent.append(params[2].getString());
            break;
        case 9:
            messageContent.append(params[0].getString());
            messageContent.append(";");
            messageContent.append(params[1].getString());
            messageContent.append(";");
            messageContent.append(params[2].getInt();
            break;
        default:
            messageContent.append(params[0].getString());
            messageContent.append(";");
            messageContent.append(params[1].getString());
            
        break;
    }
    _message.setMessage(messageContent.c_str(), messageContent.size());
}

void MessageDecoder::decode(Message& _message, std::vector<Parameter>& params, int& _operation, MessageType& _type){
    _operation = _message.getOperation();
    _type = _message.getMessageType();
    std::vector<std::string> tokens;

    char* pch = strtok (_message.getMessage(),";");
    while (pch != NULL)
    {
    	tokens.push_back(pch);
    	pch = strtok (NULL, " ,.-");
    }

    switch(_operation){
        case 5:
        break;
        case 1 || 2 || 8:
            Parameter param1;
            param1.setString(tokens[0]);
            params.push_back(param1);
        break;
        case 3 || 4 || 9:
        break;
        case 6 || 7:
            Parameter param1;
            std::vector<std::string> _vs;
            for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
                _vs.push_back(*it);
            }
            param1.setVectorString(_vs);
            params.push_back(param1);
        break;
        case 10:
            Parameter param1;
            param1.setBoolean(tokens[0]);
            params.push_back(param1);            
        break;
    }

}
