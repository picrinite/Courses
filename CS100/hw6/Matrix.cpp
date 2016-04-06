// Course:            CS 100
// =======================================================================
#include "Matrix.h"
template < typename Element >
Matrix<Element>::Matrix( int newRows, int newCols )
	: rows( newRows ), cols( newCols ), m( rows ){
	for (int i = 0; i < rows; i++ )
		m[i] = new Array < Element >( cols );
}
template < typename Element >
int Matrix<Element>::numRows(){
	return rows;
}
template < typename Element >
int Matrix<Element>::numCols(){
	return cols;
}
template < typename Element >
Array < Element > & Matrix<Element>::operator [] ( int row )
	throw(IndexOutOfBoundsException){
	if( row >= 0 && row < rows )
		return * m[row];
	throw(IndexOutOfBoundsException("Matrix: row number out of range"));
}
template < typename Element >
void  Matrix<Element>::print( ostream & out ){
	for(int i = 0; i < rows; ++i){ 
		out << "row " << i << ": ";
		out << *m[i];
	}
}
//use a different template name for friend function, otherwise, it is not a template function after compiling
template <typename T>
std::ostream & operator << ( ostream & out, Matrix<T> & m ){
	m.print( out );
	return out;
}