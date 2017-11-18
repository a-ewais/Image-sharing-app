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

enum MessageType {Request, Reply , Ack};
static int rpc_count = 0;
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
		Message(MessageType type); //Done
		Message(Message* big, int _part_num,int _size); //Done
//		Message(int operation, char * p_message, size_t p_message_size, int p_rpc_id);
		Message(char * marshalled_base64); //Done
//		Message(char * mess, int size);
		Message(std::vector<Message *>& parts);	//Done
		std::vector <Message*> split(int);  //Done
		bool isComplete();
		char * marshal (int& size);	//Done
		int getPartsNum();	//Done
		int getPartNum();	//Done
		int getOperation ();	//Done
		int getRPCId();		//Done
		char * getMessage();	//Done
		size_t getMessageSize();	//Done
		MessageType getMessageType();	//Done
		void setOperation (int _operation); //Done
		void setMessage (char * message, size_t message_size); //Done
		void setMessageType (MessageType message_type); //Done
		~Message();
};

#endif /* MESSAGE_H_ */
