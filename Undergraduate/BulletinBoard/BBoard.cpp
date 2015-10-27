#include "BBoard.h"
#include "Reply.h"
#include "Topic.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;
BBoard::BBoard(){
	current_user=NULL;
}
BBoard::BBoard(const string &ttl){
	title=ttl;
	current_user = NULL;
}
bool BBoard::load_users(const string &userfile){
	ifstream fIn( userfile.c_str() );
	if( fIn.is_open() ) {
		string temp1,temp2;
		fIn>>temp1;
		while(temp1!="end"){
			fIn>>temp2;
			user_list.push_back(User(temp1,temp2));
			fIn>>temp1;
		}
		fIn.close();
		return true;
	}
	else
		return false;
}
bool BBoard::load_messages(const string& datafile){
	ifstream fIn( datafile.c_str() );
	if( fIn.is_open() ) {
		string tmp, strBegin;
		string author, subject;
		unsigned id = 0;
		vector< string > children;
		getline( fIn, tmp ); //pass over the first line , number of messages
		while( fIn.good() ){
			getline( fIn, strBegin );
			if( strBegin[ strBegin.size() -1 ] == '\r' )
				strBegin.erase( strBegin.size() -1 );

			getline( fIn, tmp );  //pass over the line with id information

			getline( fIn, subject);
			if( subject[ subject.size() -1 ] == '\r' )
				subject.erase( subject.size() -1 );
			subject.erase( 0, 10);

			getline( fIn, author);
			if( author[ author.size() -1 ] == '\r' )
				author.erase( author.size() -1 );
			author.erase( 0, 7);

			string body;
			string str_children;
			getline( fIn, tmp ); //tmp may be children or body
			if( tmp[ tmp.size() -1 ] == '\r' )
				tmp.erase( tmp.size() -1 );
			if( tmp.find( "children", 0 ) != string::npos ){
				tmp.erase( 0, 11);
				children.push_back(tmp);
				getline( fIn, tmp );
				if( tmp[ tmp.size() -1 ] == '\r' )
					tmp.erase( tmp.size() -1 );
			}
			else 
				children.push_back("");
			while( tmp != "<end>" ) {
				body = body + '\n' + tmp;
				getline( fIn, tmp );
				if( tmp[ tmp.size() -1 ] == '\r' )
					tmp.erase( tmp.size() -1 );
			}
			++id;
			//cout << id << endl;
			body.erase( 0, body.find_first_of(":", 1));  //remove the first '\n'
			body.erase( 0, body.find_first_of(":", 1) + 2 ); //remove :body:
			//cout << body << endl;
			if ( strBegin == "<begin_topic>" ){
				//cout << " strBegin :" << strBegin << endl;
				//cout << " size :" << strBegin.size() << endl;
				Message *mg = new Topic( author, subject, body, id);
				//cout << "topic added " << endl;
				message_list.push_back( mg );
			}
			else {
				//cout << " strBegin :" << strBegin << endl;
				//cout << " size :" << strBegin.size() << endl;
			    Message *mg = new Reply( author, subject, body, id);	
				//cout << "reply added " << endl;
				message_list.push_back( mg );
			}
		}
		fIn.close();
		int child;
		for( unsigned i = 0; i < children.size(); ++i ){
			stringstream iss( children[i] );
			while( iss >> child ){
				message_list[ i ] -> add_child( message_list[ child - 1] );
				//cout << child << " is added" << endl;
			}	
		}
		return true;
	}
	else
		return false;
}
bool BBoard::save_messages(const string& datafile){
	ofstream fOut( datafile.c_str() );
	if( fOut.is_open() ) {
		fOut << message_list.size();
		for( unsigned i = 0; i < message_list.size(); ++i )
			fOut << endl << message_list[ i ]->to_formatted_string() ;
		fOut.close();
		return true;
	}
	else
		return false;
}
const User * BBoard::get_user(const string &name, const string &pw) const{
	for( unsigned i = 0; i < user_list.size(); ++i ){
		if( user_list[i].check(name,pw) )
			return &user_list[i];
	}
	return NULL;
}
void BBoard::login(){
	string name, pass;
	cout << endl << "Welcome to Jack's Amazing Bulletin Board" << endl;
	while(true){
		cout<<"Enter your username (\"Q\" or \"q\" to quit):";
		getline(cin,name);
		if(name.compare("Q")==0 || name.compare("q")==0){
			cout<<"Bye!"<<endl;
			return;
		}
		cout<<"Enter your password:";
		getline(cin,pass);
		current_user = get_user( name, pass );
		if( current_user )
			break;
		cout<<"Invalid Username or Password!"<<endl<<endl;
	}
}

void BBoard::add_reply(){
	string subject, body, tmp;
	int id;
	int message_id;
	while( true ){
		cout << endl << "Enter Message ID (-1 for Menu): " ;
		getline( cin, tmp);
		stringstream ss( tmp );
		ss >> message_id;
		//cin >> message_id;            //****** use this will make getline( cin, tmp ) doesn't work
		if( message_id == -1 )
			return;
		if( message_id > 0 && message_id <= (int)message_list.size() )
			break;
		cout << "Invalid Message ID!!" << endl;
	}
	id = message_list.size() + 1;
	subject = "Re: " + message_list[ message_id - 1 ]->get_subject();
	cout << "Enter Body: ";
	getline( cin, tmp);
	if( tmp == "" ){
		body = body + '\n';
		getline( cin, tmp);
	}
	while ( tmp != "" ){
		body = body + '\n' + tmp;
		getline( cin, tmp );
	}
	body.erase( 0, 1 ); //remove the first character '\n' 
	Message *mg = new Reply( current_user->get_username(), subject, body, id );
	message_list.push_back( mg );
	message_list[ message_id - 1 ]->add_child( mg );
}
void BBoard::add_topic(){
	string subject, body, tmp;
	int id;
	id = message_list.size() + 1;
	cout << "Enter Topic: ";
	getline( cin, subject );
	cout << "Enter Body: ";
	getline( cin, tmp);
	if( tmp == "" )
		getline( cin, tmp);
	while ( tmp != "" ){
		body = body + '\n' + tmp;
		getline( cin, tmp );
	}
	body.erase( 0, 1 ); //remove the first character '\n' 
	Message *mg = new Topic( current_user->get_username(), subject, body, id );
	message_list.push_back( mg );
}
//display should be rewritten
void BBoard::display() const{
	if( message_list.size()==0 )
		cout<<endl<<"Nothing to Display."<<endl;
	else {
		cout<< endl << "-------------------------------------------------------------------------------"<<endl;
		for( unsigned i = 0;i < message_list.size(); ++i){
			//cout << " I have been here " << endl;
			if( !message_list[i]->is_reply() ){
				message_list[i]->print( 0 );	
				cout<<"-------------------------------------------------------------------------------"<<endl;
			}
		}
	}
}
void BBoard::run(){
	if( !current_user )
		return;
	cout<< endl << "Welcome back "<<current_user->get_username()<<"!"<<endl;
	while(true){
		cout<< endl << "Menu"<<endl;
		cout<<" - Display Messages ('D' or 'd')"<<endl;
		cout<<" - Add New Topic ('N' or 'n')"<<endl;
		cout<<" - Add Reply to a Topic ('R' or 'r')"<<endl;
		cout<<" - Quit ('Q' or 'q')"<<endl;
		cout<<"Choose an action:";
		//cin.ignore(100,'\n');
		//char c;
		//cin>>c;
		string s;
		getline(cin,s);
		if(s=="N" || s=="n")
			add_topic();
		else if(s=="R" || s=="r")
			add_reply();
		else if(s=="D" || s=="d")
			display();
		else if(s=="Q" || s=="q"){
			cout<<"Bye!"<<endl;
			return;
		}
	}
}
BBoard::~BBoard( ){
	//delete current_user;
	for( unsigned i = 0; i < message_list.size(); ++i ){
		if( message_list[i] ) 
			delete message_list[i];
	}
}