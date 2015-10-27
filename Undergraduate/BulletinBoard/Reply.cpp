#include "Reply.h"
#include <sstream>
Reply::Reply(){
}
Reply::Reply(const string &athr, const string &sbjct, const string &bdy, unsigned i){
		author = athr;
		subject = sbjct;
		body = bdy;
		id = i;
}
bool Reply::is_reply() const{
	return true;
}
string Reply::to_formatted_string() const{
	string s("<begin_reply>");
	s +='\n';
	stringstream ss;
	ss << id;
	s += ":id: " + ss.str()+ '\n';
	s += ":subject: " + subject + '\n';
	s += ":from: " + author + '\n';
	if( !child_list.empty() ){
		s += ":children:";
		for( unsigned i = 0; i < child_list.size(); ++i ){
			stringstream ssid;
			ssid << child_list[ i ]->get_id();
			s += ' ' + ssid.str();
		}
		s += '\n';
	}
	s += ":body: " + body + '\n';
	s += "<end>";
	return s;
}
