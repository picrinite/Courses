#ifndef TOPIC_H
#define TOPIC_H
#include "Message.h"
class Topic: public Message{
public:
	Topic();
	Topic(const string &athr,
	const string &sbjct,
	const string &bdy,
	unsigned id);
	bool is_reply() const;
	string to_formatted_string() const;
};
#endif