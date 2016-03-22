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
void parse( char * str, char **argv, char *& input_file, char*& output_file, vector<int> & pos_argv ){
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
/*
void exeNoPipe( char ** chargv, char* input_file, char* output_file ){
	int status = 0;
	pid_t child_pid, wpid;
	child_pid = fork();
	switch( child_pid ){
	case 0: 
		if( inputExist ){
			int filedesc = open( input_file, O_RDONLY );
			if ( filedesc < 0 ) {
				perror( "open( input_file, O_RDONLY ) ");
				exit(1);
			}
			if( dup2( filedesc, 0 ) < 0 ){
				perror( "dup2( filedesc, 0 )");
				exit(1);
			}
		}
		if( outputExist ){
			int filedesc = open( output_file, O_WRONLY );
			if ( filedesc < 0 ) {
				perror( "open( output_file, O_WRONLY ) ");
				exit(1);
			}
			if( dup2( filedesc, 1 ) < 0 ){
				perror( "dup2( filedesc, 1 )");
				exit(1);
			}
		}
		if( execvp(chargv[0], chargv  ) < 0 ){
			perror( "execvp(chargv[0], chargv) ");
			exit(1);
		}
		break;
	case -1:
		perror("fork");
		exit (1);
	default:
		signal(SIGINT,SIG_IGN);
		wpid = wait(&status);
		if( wpid < 0 ){
			perror( "wait(&status) ");
			exit(1);
		}
		signal(SIGINT,SIG_DFL);
		break;
	}	
	cout << endl;
}*/
void runCommand( char * inbuf, char ** chargv, char * input_file, char* output_file){
	vector<int> pos_argv;
	pos_argv.push_back( -1 );  // assume the first | symbol is at position -1,   pos_argv records all the index of char * pointed to "|" in char ** chargv
	                         // cat <a.txt | tee b.txt  , has four char* in chargv as {"cat", "|" , "tee", "b.txt"}, pos_argv would be composed of {-1 , 1 }
							//  cat | | b.txt  , has four char* in chargv as {"cat", "|" , "|", "b.txt"}, pos_argv would be composed of {-1 , 1, 2 }
	parse(inbuf, chargv, input_file, output_file, pos_argv);
	/*int size_chargv = pos_argv.back();
	for( int i = 0; i < size_chargv ; ++i )   // test parse
		if(chargv[i])
		cout << chargv[i] << " " << flush;*/
	//cout << endl << "INPUT:" << input_file << endl;
	//cout << "OUTPUT: " << output_file << endl;
	/*for( int i = 0; i < pos_argv.size(); ++i ){
		cout << pos_argv[i] << endl;
	}*/
	/*for( int i = 0; i < pos_argv.size() - 1; ++i ){	    // test parse
		for( int j =  pos_argv[i] + 1 ; chargv[j] ; ++j )   
			cout << chargv[j] << " " << flush;
		cout << endl;
	}*/
	int num_cmd = pos_argv.size() - 1;
	int num_pipe =  num_cmd - 1;        //cout << "num_pipe: " << num_pipe << endl;
	/*if( num_cmd == 1 ){  // this can be handled by the while loop
		exeNoPipe( chargv, input_file, output_file );
		return;
	}*/
	vector<int*> pipeContainer;
	for( int i = 0; i < num_pipe; ++i ){
		//int ioPipe[2];    // this is WRONG
		int *ioPipe = new int[2];
		pipeContainer.push_back(ioPipe);
		if( pipe( pipeContainer.back() ) < 0 ){
			perror( "pipeContainer.back() ");
			exit(1);
		}
		//cout << "i: " << pipeContainer.back()[0] << endl;
		//cout << "i: " << pipeContainer.back()[1] << endl;
		//cout << "Pointer position: " << pipeContainer.back() << endl;
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
			cout << "enter child process, index of cmd is " << index << endl;
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
			//	close(  pipeContainer[index-1][0] );
			//	close(  pipeContainer[index-1][1] );
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
					//close( filedesc );
				}
			}
			else{           //first or middle command
				if( dup2( pipeContainer[index][1], 1 ) < 0 ){
					perror( "dup2( pipeContainer[index][1], 1 ) ");
					exit(1);
				}
			//	close(  pipeContainer[index-1][1] );
				//close(  pipeContainer[index][1] );
			}
			start = pos_argv[index] + 1;
			//cout << "start : " << start << endl;
			//Not necessary to close all pipes in child process, different from reference http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
			// because  pipeContainer[i][1] is inherited from parent, which is already closed, no need to close in child process again.
			// so both the writer file descriptor at pipe[index-1] is closed after fork()
			// exec() process will read from kernel and close immediately
			/*for( int i = 0; i < num_pipe; ++i ){
				close( pipeContainer[i][0]);
				close( pipeContainer[i][1]);
				cout << "cmd "<< index << " child closed pipe " << i << endl;
			}*/
			if( execvp( chargv[start], chargv + start ) < 0 ){   // will first read from stdin( which has been duplicated to pipe[index-1][0], 
																//if no writer file descriptor open, executed program will close
				perror( "execvp( chargv[start], chargv + start )");
				exit(1);
			}
			break;
		case -1:
			perror("fork");
			exit (1);
			break;
		default:
			//wait(&status);
			pid_t wpid;
			while ((wpid = wait(&status)) > 0){
				if (WIFEXITED(status))
					cout << endl << "Parent detected child process " << wpid << " is done." << endl;
			}
			// All pipe[0] (read) in parent process can be left open
			/*if( index != 0 ) {
				close( pipeContainer[index-1][0] );
				cout << " parent close: " << index -1 << "  0 " << endl;
				//close( pipeContainer[index-1][1] );
			}*/
			// pipeContainer[index][1] in parent should be closed, otherwise, child process will wait writer in parent to close (hang) 
			// what happened after fork()?
			// in child process, pipeContainer[index][0] is inherited open,  pipeContainer[index][1] is inherited close,
			// exec() in child process will read kernel through pipeContainer[index][0]
			if( index != num_cmd - 1 ){
				close( pipeContainer[index][1]);
				cout << " parent close: " << index << "  1 " << endl;
			}
			++index;
			break;
		}
	}
	//Below method doesn't work, close() specifically in parent while loop
	/*
	pit_d wpid;
	while ((wpid = wait(&status)) > 0){
		if (WIFEXITED(status))
		   cout << endl << "Parent detected child process " << wpid << " is done." << endl;
	}
	for( int i = 0; i < num_pipe; ++i ){
		close( pipeContainer[i][0]);
		close( pipeContainer[i][1]);
		//cout << "parent closed " << i << endl;
	}*/
	for( int i = 0; i < num_pipe; ++i ){
		delete [] pipeContainer[i];
	}
	cout << endl;
}
int main(){
	char inbuf[MAX_BUFFER];
	char *input_file = NULL; 
	char *output_file = NULL; 
    char *chargv[64];
	while ( true ) {
		cout << "% ";
		gets(inbuf);
		if (strcmp(inbuf, "q") == 0 || strcmp(inbuf, "Q") == 0 )
			break;
		runCommand( inbuf, chargv, input_file, output_file );
		inputExist = false;   // '<' appears in command line
		outputExist = false; // '>' appears in command line
	}   
	return 0;
}