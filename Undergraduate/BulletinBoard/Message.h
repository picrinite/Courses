#ifndef _MESSAGE_H
#define _MESSAGE_H
#include <string>
#include <vector>
using namespace std;
class Message
{
protected:
	string author;
	string subject;
	string body;
	unsigned id;
	vector<Message*> child_list;
public:
	//default constructor
	Message();
	//Constructor with parameters
	Message(const string &athr,
		const string &sbjct,
		const string &bdy,
		unsigned id);
	virtual bool is_reply() const = 0;
	virtual string to_formatted_string() const = 0;
	void print(unsigned indentation) const;
	//returns the subject string.
	const string & get_subject() const;
	unsigned get_id() const;
	void add_child(Message *child);
	virtual ~Message();
};
#endif