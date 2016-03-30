// Course:            CS 100
#include "Exception.h"
#include "Array.cpp"  // if #include "Array.h"  , linking error
#include <iostream>
using namespace std;
#ifndef MATRIX_H
#define MATRIX_H
template < typename Element >
class Matrix
{
private:
	int rows, cols;
	Array<Array<Element>* > m;
	//Another method:
	/*Array<Element > *m;
	Matrix( int newRows, int newCols )
	: rows( newRows ), cols( newCols ), m( new Array<Element>[rows] ){ // initialized as array of Array<Element>
	for (int i = 0; i < rows; i++ )
		m[i] = Array < Element >( cols );  // assign each element in m
	}
	*/
public:
	Matrix( int newRows, int newCols );
	int numRows();
	int numCols();
	Array < Element > & operator [] ( int row ) throw(IndexOutOfBoundsException);
	void print( ostream & out );
	template <typename T>  // use a different template name for friend function
	friend std::ostream & operator << ( ostream & out, Matrix<T> & m );
};
#endif