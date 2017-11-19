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
    void encode(Message& _message, std::vector<Parameter>&, int&, MessageType&);
    void decode(Message& _message, std::vector<Parameter>&, int&, MessageType&);
};

#endif