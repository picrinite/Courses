#include "User.h"
#include <string>
using namespace std;
User::User(){
	username="";
	password="";
}
	// creates a user with given username and password.
User::User(const string& uname, const string& pass){
	username=uname;
	password=pass;
}
	//returns the username
string User::get_username() const{
	return username;
}
	// returns true if the stored username/password matches with the
	// parameters. Otherwise returns false.
	// Note that, even though a User with empty name and password is
	// actually a valid User object (it is the default User), this
	// function must still return false if given a empty uname string.
bool User::check(const string &uname, const string &pass) const{
	if(uname.empty()||pass.empty())
		return false;
	return username==uname && password==pass;
}