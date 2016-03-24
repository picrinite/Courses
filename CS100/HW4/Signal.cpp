// Course:            CS 100
//  I certify that all the codes are my own original work.
// =======================================================================
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
using namespace std;
struct sigaction act;
void sigHandle(int sig)
{
	static int i = 0;
	static int q = 0;
	static int s = 0;
	switch( sig ){
	case SIGINT:           //terminal interrupt, ctrl c
		cout << "I" << endl;
		++i;
		break;
	case SIGQUIT:      //terminal quit  ^\
		cout << "Q" << endl;
		++q;
		break;
	case SIGTSTP:        //stop executing ^Z
		++s;
		if( s == 3 ){
			cout << endl << "Interrupt: " << i << endl;
			cout << "Stop: " << s << endl;
			cout << "Quit: " << q << endl;
			exit (0);
		}
		cout << "S" << endl;
		signal(SIGTSTP, SIG_DFL);
		// Use either kill or raise to stop
		//kill( getpid(), SIGTSTP );   
		if ( raise( SIGTSTP ) ){           //  raise() returns 0 on success, and nonzero for failure.
			perror( "raise( SIGTSTP ): ");
			exit (1);
		}
		break;
	case SIGCONT:
		if ( sigaction(SIGTSTP, &act, 0) == -1 ) {
			perror( "sigaction(SIGTSTP, &act, 0) : " );
			exit (1);
		}
		break;
	default:
		break;
	}
}
void init_signal_handlers(){
// siginfo_t will not be used , otherwise,  
		//act.sa_flags  = 0;
		//act.sa_sigaction = sigHandle( int signum, siginfo_t *siginfo, void *uctx);
	act.sa_flags = 0;
	act.sa_handler = sigHandle;
	// make the set contain ALL signals available on our system //
	sigfillset(&act.sa_mask);
	//Initialization
	if ( sigaction(SIGINT, &act, 0) == -1 ) {
		perror( "sigaction(SIGINT, &act, 0) : " );
		exit (1);
	}
	if ( sigaction(SIGQUIT, &act, 0) == -1 ) {
		perror( "sigaction(SIGQUIT, &act, 0) : " );
		exit (1);
	}
	if ( sigaction(SIGTSTP, &act, 0) == -1 ) {
		perror( "sigaction(SIGTSTP, &act, 0) : " );
		exit (1);
	}
	if ( sigaction(SIGCONT, &act, 0) == -1 ) {
		perror( "sigaction(SIGCONT, &act, 0) : " );
		exit (1);
	}
}
int main()
{
	init_signal_handlers();
	while(1) {
		cout << "X" << flush ; 	
		usleep(2000);  // use 2 milisedond to make the testscript readable
	}
}
// http://www.alexonlinux.com/signal-handling-in-linux#signal_masks
/*
struct sigaction{
	void (*sa_handler)(int signum);
	void (*sa_sigaction)(int signum, siginfo_t *siginfo,
	void *uctx);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void);
};
*/
