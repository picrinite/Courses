#include "Date.h"
void test1()
{
	cout << "1. Default" << endl;
	Date x = Date();
	cout << "numeric:\t" ;
	x.printNumeric(); cout << endl;
	cout << "alpha:\t";
	x.printAlpha();
	cout << endl;
	cout << "--------"<< endl;
}
void test2()
{
	cout << "2. Numeric Date: 7.3.1991" << endl;
	Date x = Date(7,3,1991);
	cout << "numeric:\t" ;
	x.printNumeric();
	cout << endl;
	cout << "alpha:\t";
	x.printAlpha();
	cout << endl;
	cout << "-------- "<< endl;
}
void test3()
{
	cout << "3. Alpha Date: July 3, 1991" << endl;
	Date x = Date("July",3,1991);
	cout << "numeric:\t" ;
	x.printNumeric();
	cout << endl;
	cout << "alpha:\t";
	x.printAlpha();
	cout << endl;
	cout << "--------"<< endl;
}
void test4()
{
	cout << "4. Invalid Numeric Date: 0.3.1991 > 1.3.1991" << endl;
	Date x = Date(0,3,1991);
	cout << "numeric:\t" ;
	x.printNumeric();
	cout << endl;
	cout << "alpha:\t";
	x.printAlpha();
	cout << endl;
	cout << "--------"<< endl;
}
void test5()
{
	cout << "5. Invalid Alpha Date: 1.adf.1991 > 1.1.2000" << endl;
	Date x = Date("adf",1, 1991);
	cout << "numeric:\t" ; x.printNumeric();
	cout << endl;
	cout << "alpha:\t";
	x.printAlpha();
	cout << endl;
	cout << "--------"<< endl;
} 
void test6()
{
	cout << "2. Numeric Date: 11.11.2015" << endl;
	Date x = Date(11,11,2015);
	cout << "numeric:\t" ;
	x.printNumeric();
	Date y=x.addDays(-1000);
	cout << endl;
	cout << "After Minus 1000 days"<<endl;
	y.printNumeric();
	cout << endl;
	cout << "-------- "<< endl;
}
int main()
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
}