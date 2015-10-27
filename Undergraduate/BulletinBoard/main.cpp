#include "BBoard.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int main(int argc, char * argv[]) 
{
	// check commandline arguments
	if (argc != 3){
		cout << "ERROR: Invalid program call." << endl
			<< "Usage: <program_name> userfile datafile" << endl;
			return 1;
	}
	string userfile = "users1.txt";
	string datafile = "data1.txt";
	string test= "data.txt";
	// test getline
	/*string tmp;
	ifstream fIn(test.c_str());
	getline( fIn, tmp, '\r');
		cout << "tmp size: " << tmp.size() << endl;*/
	BBoard bb("CS12 Bulletin Board");
	// load users from file
	if (!bb.load_users( argv[1] ))
	{
		cout << "ERROR: Cannot load users from " << argv[1]  << endl;
			return 1;
	}
	//load messages
	if (!bb.load_messages( argv[2] ))
	{
		cout << "ERROR: Cannot load messages from " << argv[2]  << endl;
			return 1;
	}
	//cout << " message number: " << bb.message_list.size() << endl;
	bb.login();
 	bb.run();
	// save messages
	if (!bb.save_messages(argv[2]))
	{
		cout << "ERROR: Cannot save messages to " << datafile << endl;
			return 1;
	}
	return 0;
}
