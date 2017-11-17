/*
 * Message.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ewais
 */

#include <cstdio>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#ifndef MESSAGE_H
#define MESSAGE_H
#include "Base64_Marshal.h"
static int count = 0;

enum MessageType {Request, Reply , Ack};

class Message
{
	private:
	//in all headers 16 byte
		MessageType message_type;
		int rpc_id;
		int parts_num;
		int part_num;
	//
		int operation;
		char * message;
		size_t message_size;

	public:
		Message(Message* big, int _part_num,int _size);
		Message(int operation, char * p_message, size_t p_message_size, int p_rpc_id);
		Message(char * marshalled_base64);
		Message(char * mess, int size);
		Message(std::vector<Message *> parts);
		std::vector <Message*> split(int);
		char * marshal (int& size);
		int getOperation ();
		int getRPCId();
		void * getMessage();
		size_t getMessageSize();
		MessageType getMessageType();
		void setOperation (int _operation);
		void setMessage (void * message, size_t message_size);
		void setMessageType (MessageType message_type);
		~Message();
};

#endif /* MESSAGE_H_ */
