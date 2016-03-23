#include <iostream>
#include <unistd.h>
using namespace std;
int main()
{
	while(1) {
		cout << "X" << flush ; 	
		usleep(2000);  // use 2 milisedond to make the testscript readable
	}
}
