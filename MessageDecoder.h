#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H

#include "Message.h"
#include "Parameter.h"
#include <vector>
#include <string>


class MessageDecoder{
private:

public:
    MessageDecoder();
    ~MessageDecoder();
    static std::string encodeIpPort(sockaddr_in raw);
    static sockaddr_in decodeIpPort(std::string s);
    static void encode(Message& _message, std::vector<Parameter>&, int, MessageType);
    static void decode(Message& _message, std::vector<Parameter>&, int&, MessageType&);
};

#endif
