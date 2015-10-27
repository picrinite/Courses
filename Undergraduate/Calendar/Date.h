#ifndef Date_H
#define Date_H
#include <iostream>
using namespace std;
class Date{
private:
	unsigned day;
	unsigned month;
	string monthName;
	unsigned year;
	bool isLeap( unsigned ) const; //Tests for leap year.
	unsigned daysPerMonth( unsigned m, unsigned y ) const;//Returns number of days allowed in a given month e.g. daysPerMonth(9, 2000) returns 30.
	string name( unsigned m ) const;   //Returns the name of a given month e.g. name(12) returns the string "December"
	unsigned number( const string &mn ) const; //Returns the number of a given named month e.g. number("March") returns 3
public:
	Date();//default constructor: creates the date January 1st, 2000.
	Date( unsigned m, unsigned d, unsigned y );//parameterized constructor: month number, day, year e.
	//g. (3, 1, 2010) would construct the date March 1st, 2010
	Date( const string &mn, unsigned d, unsigned y );//parameterized constructor: month name, day, year e.g. (December, 15, 2012) would construct
	//the date December 15th, 2012
	void printNumeric( ) const;//const accessor function to output a Date exactly in the format "3/1/2012".
	//Do not output a newline (endl) at the end!
	void printAlpha( ) const;//const accessor function to output a Date exactly in the format "March 1, 2012". The first
	//letter of the month name is upper case, and the month name is printed in full so Jan, january, jan are not valid formats.
	//Do not output a newline (endl) at the end!
	Date addDays( int ) const;
};
#endif