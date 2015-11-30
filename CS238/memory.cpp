#include <sys/resource.h>
#include <iostream>
//#include <string.h>
//#include <stdlib.h>
//#include <unistd.h>
 using namespace std;
int main() {

	struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);
	cout << "Maximal memory usage at the beginning of the program: " << r_usage.ru_maxrss << endl;
	int *arr = new int[600*600];
	  for( int i = 0; i <360000; ++i )
	  arr[i] = 0;
	//int i;
	getrusage(RUSAGE_SELF,&r_usage);
	cout << "Maximal memory usage at the end of the program: " << r_usage.ru_maxrss << endl;
	return 0;
}
   //getrusage(RUSAGE_SELF,&r_usage);
  /*while (++i <= 10) {
    void *m = malloc(20*1024*1024);
    memset(m,0,20*1024*1024);
    getrusage(RUSAGE_SELF,&r_usage);
    printf("Memory usage = %ld\n",r_usage.ru_maxrss);
    sleep (3);
  }*/
  /*int *arr = new int[ 20*1024*1024];
  for( int i = 0; i < 20*1024*1024; ++i )
	  arr[i] = 0;
  
    printf("Memory usage = %ld\n",r_usage.ru_maxrss);
  printf("\nAllocated memory, sleeping ten seconds after which we will check again...\n\n");*/
 // delete [] arr;
  //sleep (3);
