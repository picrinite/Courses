#include "Message.h"
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;
Message::Message(){
}
Message::Message(const string &athr,const string &sbjct,const string &b, unsigned i){
	author = athr;
	subject = sbjct;
	body = b;
	id = i;
}
void Message::print(unsigned indentation) const{
	if( indentation !=0 )
		cout << endl;
	for( unsigned i = 0; i < indentation ; ++i )
		cout << "  ";
	cout << "Message #" << id << ": " << subject << endl;
	for( unsigned i = 0; i < indentation ; ++i )
		cout << "  ";
	string newbody(body);
	unsigned pos = newbody.find_first_of( '\n', 0 );
	while( pos < newbody.size() ){
		newbody.insert( pos + 1 , (int)indentation * 2, ' ' );  //insert before position pos + 1
		pos = newbody.find_first_of( '\n', pos + 1 );
	}
	cout << "from " << author << ": " << newbody << endl;
	//cout << child_list.size() << endl;
	++indentation;
	for( unsigned i = 0; i < child_list.size(); ++i )
		child_list[ i ]->print( indentation );
}
const string & Message::get_subject() const{
	return subject;
}
unsigned Message::get_id() const{
	return id;
}
void Message::add_child(Message *child){
	child_list.push_back( child );
}
Message::~Message(){
	for( unsigned i = 0; i < child_list.size(); ++i )
		child_list[ i ] = NULL;
}