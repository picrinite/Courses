#ifndef _BBOARD_H
#define _BBOARD_H
#include <string>
#include <vector>
#include "User.h"
#include "Message.h"
using namespace std;
class BBoard{
private:
	string title;
	vector<User> user_list;
	const User *current_user;
	vector<Message*> message_list;
	/*It traverses the user_list, testing for the existence of a user with the given name and
	password; if this user is NOT found, the function returns NULL; otherwise it returns a
	pointer to the identified User*/
	const User * get_user(const string &name, const string &pw) const;
	void display() const;
	void add_topic();
	void add_reply();
public:
	BBoard();
	BBoard(const string& ttl);
	bool load_users(const string& userfile);
	bool load_messages(const string& datafile);
	bool save_messages(const string& datafile);
	void login();
	void run();
	~BBoard( );
};
#endif