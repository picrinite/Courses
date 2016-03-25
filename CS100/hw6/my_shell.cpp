// Course:            CS 100
// First Name:        Xing
// Last Name:         Zhang
// ID Number:         860935445
// Email address:     xzhan018@ucr.edu
// TA:                Mike Izbicki
//  I certify that all the codes are my own original work.
// =======================================================================
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>  // open()
#include <cstring>
#include <stdio.h>
#include <vector>
using namespace std;
const int MAX_BUFFER = 1024;
bool inputExist = false;   // '<' appears in command line
bool outputExist = false; // '>' appears in command line
bool backgound_job = false;
void parse( char * str, char **argv, char *& input_file, char*& output_file, vector<int> & pos_argv ){   // parse command, save in char **argv
	int j = 0;
	bool startFound = false;  // whether the first char in any argument is found
	bool inputFound = false;   // '<' found
	bool outputFound = false; // '>' found
	for( int i = 0; str[i]; ++i ){
		char& ch_c = str[i];
		if( ch_c != ' ' && ch_c != '\t' && ch_c != '>' && ch_c != '<' && ch_c != '|'  ){
			if ( startFound )
				continue;
			if( inputFound ){
				input_file = str + i;
				inputFound = false;
			}
			else if( outputFound ){
				output_file = str + i;
				outputFound = false;
			}
			else {
				argv[j++] = str + i;
			}
			startFound = true;
		}
		else {
			if( ch_c == '&' ){             // & represents background job
				backgound_job = true;
			}
			if( ch_c == '<' ){
				inputFound = true;
				inputExist = true;			
			}
			else if( ch_c == '>' ){
				outputFound = true;
				outputExist = true;
			}
			else if( ch_c == '|' ){
				pos_argv.push_back( j );
				argv[j++] = NULL;
			}
			ch_c = '\0';
			startFound = false;
		}
	}
	argv[j] = 0;
	pos_argv.push_back( j );
}
void parsePath( char * str, char ** pathNames ){  //parse $PATH
	int j = 0;
	bool startFound = false;  // whether the first char in any path is found
	for( int i = 0; str[i]; ++i ){
		char& ch_c = str[i];
		if( ch_c != ':' ){
			if ( startFound )
				continue;
			else {
				pathNames[j++] = str + i;
			}
			startFound = true;
		}
		else {
			ch_c = '\0';
			startFound = false;
		}
	}
	pathNames[j] = NULL;
}
void runCommand( char ** chargv, vector<int> & pos_argv, char * input_file, char* output_file, char ** pathNames){
	int num_cmd = pos_argv.size() - 1;
	int num_pipe =  num_cmd - 1;        //cout << "num_pipe: " << num_pipe << endl;
	vector<int*> pipeContainer;
	for( int i = 0; i < num_pipe; ++i ){
		//int ioPipe[2];    // this is WRONG
		int *ioPipe = new int[2];
		pipeContainer.push_back(ioPipe);
		if( pipe( pipeContainer.back() ) < 0 ){
			perror( "pipeContainer.back() ");
			exit(1);
		}
	}                 
	int status = 0;
	pid_t child_pid;
	int index = 0;
	int filesrc, filedesc;
	while( index < num_cmd ){    // fork for each command, command's index start from 0 .    "cat scores | grep Villanova | cut -b 1-10"  index = 0, 1, 2
		int start;
		child_pid = fork();
		switch( child_pid ){
		case 0:
			signal(SIGINT,SIG_DFL);
			if( index == 0 ){  // the first command
				if ( inputExist ){
					filesrc = open( input_file, O_RDONLY );
					if ( filesrc < 0 ) {
						perror( "open( input_file, O_RDONLY ); ");
						exit(1);
					}
					if( dup2( filesrc, 0 ) < 0 ){
						cout << "worng index: " << index << endl;
						perror( "dup2( filesrc, 0 )");
						exit(1);
					}
					//close( filesrc );
				}
			}
			else {              // middle or last command
				if( dup2( pipeContainer[index-1][0], 0 ) < 0 ){
					cout << "worng index: " << index << endl;
					perror( "dup2( pipeContainer[index-1][0], 0 )");
					exit(1);
				}
			}
			if( index == num_cmd - 1 ) {   // the last command
				if( outputExist ){
					filedesc = open( output_file, O_WRONLY );
					if ( filedesc < 0 ) {
						perror( "open( output_file, O_WRONLY ) ");
						exit(1);
					}
					if( dup2( filedesc, 1 ) < 0 ){
						perror( "dup2( filedesc, 1 )");
						exit(1);
					}					
				}
			}
			else{           //first or middle command
				if( dup2( pipeContainer[index][1], 1 ) < 0 ){
					perror( "dup2( pipeContainer[index][1], 1 ) ");
					exit(1);
				}			
			}
			start = pos_argv[index] + 1;
			for( int i = 0; pathNames[i]; ++i ) {
				char temPath[MAX_BUFFER];
				strcpy( temPath, pathNames[i] );
				strcat( temPath, "/" );
				strcat( temPath, chargv[ start ] );
				if( execv( temPath, chargv + start ) < 0 
					&&  pathNames[i+1] == NULL  ) {   // will first read from stdin( which has been duplicated to pipe[index-1][0], 
														//if no writer file descriptor open, executed program will close
					perror( "execv( temPath, chargv + start, )");
					exit(1);
				}
			}
			break;
		case -1:
			perror("forkahah");
			exit (1);
			break;
		default:
			pid_t wpid;
			while ((wpid = wait(&status)) > 0){		
				if( index != num_cmd - 1 ){
					close( pipeContainer[index][1]);
					//cout << " parent close: " << index << "  1 " << endl;
				}
				++index;
				break;
			}
		}
	}
	for( int i = 0; i < num_pipe; ++i ){
		delete [] pipeContainer[i];
	}
}
int main(){
	char inbuf[MAX_BUFFER];
	char *input_file = NULL; 
	char *output_file = NULL; 
    char *chargv[64];
	char *path;
	char *pathNames[64];
	path = getenv("PATH");
	parsePath( path, pathNames);
	while ( true ) {
		signal(SIGINT,SIG_IGN);
		cout << "% ";
		gets(inbuf);
		if (strcmp(inbuf, "q") == 0 || strcmp(inbuf, "Q") == 0 )
			break;	
		vector<int> pos_argv;
		pos_argv.push_back( -1 );  // assume the first | symbol is at position -1,   pos_argv records all the index of char * pointed to "|" in char ** chargv
	                         // cat <a.txt | tee b.txt  , has four char* in chargv as {"cat", "|" , "tee", "b.txt"}, pos_argv would be composed of {-1 , 1 }
							//  cat | | b.txt  , has four char* in chargv as {"cat", "|" , "|", "b.txt"}, pos_argv would be composed of {-1 , 1, 2 }
		parse(inbuf, chargv, input_file, output_file, pos_argv);
		runCommand( chargv, pos_argv, input_file, output_file, pathNames );
		cout << endl;
		inputExist = false;   // '<' appears in command line
		outputExist = false; // '>' appears in command line			
	}   
	return 0;
}