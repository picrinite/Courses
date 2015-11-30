//- by Xing Zhang, May 2014
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <sys/resource.h>
#include "Timer.h"
using namespace std;
const int MATCH = 5;
const int MISMATCH = -4;
const int INDEL = -8;
unsigned numPerfectMatch = 0;
int score;
//dynamically allocated 2D matrix in the form of 1D array, which is used to store the scores in one layer( z - axis ) 
struct Space{
	int lenX;  // x axis, num of columns
	int lenY;  // y axis, num of rows
	int *arr;
	Space( int x, int y ): lenX( x ), lenY( y ), arr( new int[ ( x + 1 ) * ( y + 1 ) ] ){ // y + 1 rows , x + 1 columns
		for( int i = 0; i <= lenX; ++i ){
			arr[ i ] = INDEL * i;
		}
		for( int i = 1; i <= lenY; ++i ){
			arr[ i * ( lenX + 1 ) ] = INDEL * i;
		}
	}
	int* operator[]( int row ){
		return arr + row * ( lenX + 1 ) ;
	}
	~Space() {
		delete [] arr;
	}
};
// dynamically allocated 3D matrix in the form of 1D array for standard O(mnk)-space dynamic programming 
//when any sequence degenerates to one or zero letter
struct IIID{             
	int lenX;  // x axis, num of columns
	int lenY;  // y axis, num of rows
	int lenZ;  // y axis, num of layers
	int *arr;
	struct IID{
		int lenX;
		int *larr; // array for this layer
		IID( int * p, int x ): lenX( x ), larr( p ) {
		}
		int* operator[]( int row ){
			return larr + row * ( lenX + 1 ) ;
		}
	};
	IIID( int x, int y, int z ): lenX( x ), lenY( y ), lenZ( z ), arr( new int[ ( x + 1 ) * ( y + 1 ) * ( z + 1 ) ] ){ 
		// y + 1 rows , x + 1 columns, z + 1 layers
	}
	IID operator[]( int layer ){
		int * larr = arr + layer * ( lenX + 1 ) * ( lenY + 1 );
		return IID( larr, lenX ) ;
	}
	~IIID() {
		delete [] arr;
	}
};
int getScore(char c1, char c2){   // this can be modified to any score function in the form of a 5x5 matrix indexed by A, C, G, T, and space'-'
	if( c1 == '-' && c2 == '-' ) 
		return 0;
	if( c1 == c2 )  //match
		return MATCH;
	if( c1 == '-' || c2 == '-' )  // indel
		return INDEL;
	return MISMATCH;   // mismatch
}
int getScores(char c1, char c2, char c3){
	return getScore( c1, c2 ) + getScore( c1, c3 ) + getScore( c2, c3 );
}
void backTracing(  string & x, string & y, string & z, int range[3][2], IIID & tracing, vector<string> &res){
	int & x_start = range[0][0];
	int & x_end = range[0][1];
	int & y_start = range[1][0];
	int & y_end = range[1][1];
	int & z_start = range[2][0];
	int & z_end = range[2][1];
	int len_x = x_end - x_start + 1;
	int len_y = y_end - y_start + 1;
	int len_z = z_end - z_start + 1;
	int k = len_z, i = len_y, j = len_x ;
	int dir;
	char chr_x;
	while( i || j || k){   // let 7 be xyz, 3: -yz, 5: x-z, 6: xy-, 1: --z, 2:-y-, 4: x--
		dir = tracing[ k ][ i ][ j ];
		if ( dir >= 4 ){
			res[0] += x[ x_start + j - 2 ];		
			if( dir == 7 ){                 // xyz
				chr_x = x[ x_start + j - 2 ];
				if( chr_x == y[ y_start + i - 2 ] && chr_x == z[ z_start + k - 2 ] )   // perfect match
					++numPerfectMatch;
			}
			--j;
		}
		else
			res[0] += '-';
		if ( dir % 2 ){
			res[2] += z[ z_start + k - 2 ];
			--k;
		}
		else
			res[2] += '-';
		dir = dir >> 1;
		if(  dir % 2  ){   
			res[1] += y[ y_start + i - 2 ];
			--i;
		}
		else
			res[1] += '-';	
	}
}
void printResult( vector<string> & res , ostream &out){
	int len = res[0].length() ;
	int num = 1;
	int index = len - 1;
	while( index >= 0 ){
		for( unsigned i = 0; i < res.size(); ++i ){
			if( !i )
				out << setw( 9 ) << num;
			else
				out << setw( 10 );
			int numCharLine = 0;
			while( numCharLine < 50 && index >= 0 ){
				if( numCharLine % 10 == 0 )
					out << " ";
				out << res[i][index];
				++numCharLine;
				--index;
			}
			out << endl;
			if( i != res.size() - 1 )
				index += numCharLine;
			else
				out << endl;
		}
		num += 50;
	}
}
void stdFillSpace ( string & x, string & y, string & z, int range[3][2], vector<string> & res){
	int & x_start = range[0][0];
	int & x_end = range[0][1];
	int & y_start = range[1][0];
	int & y_end = range[1][1];
	int & z_start = range[2][0];
	int & z_end = range[2][1];
	int len_x = x_end - x_start + 1;
	int len_y = y_end - y_start + 1;
	int len_z = z_end - z_start + 1;
	IIID tracing( len_x, len_y, len_z );
	IIID score( len_x, len_y, len_z );
	score[0][0][0] = 0;
	char chr_k, chr_i, chr_j;
	for( int k = 0; k <= len_z; ++k ){     // x: index j, y: index i ,  z: index k	
		if( k )
			chr_k =  z[ z_start + k - 2 ] ;
		for( int i = 0; i <= len_y; ++i ){
			if( i )
				chr_i =  y[ y_start + i - 2 ] ;
			for( int j = 0; j <= len_x; ++j ){
				int max = INT_MIN;
				int tmp;
				int arrow = 0;    // let 7 be xyz, 3: -yz, 5: x-z, 6: xy-, 1: --z, 2:-y-, 4: x--
				if( j )
					chr_j =  x[ x_start + j - 2 ];
				if( j && i && k ){    // xyz, 111(2 ) = 7(10)
					max = score [ k - 1 ][ i - 1 ][ j - 1 ] + getScores( chr_j, chr_i, chr_k );
					arrow = 7;  
				}	
				if( i && k ){     // 3: -yz
					tmp = score [ k - 1 ][ i - 1 ][ j ] + getScores( '-',  chr_i, chr_k );
					if( tmp > max ){
						max = tmp;
						arrow = 3;
					}
				}
				if( j && k ){     // 5: x-z
					tmp = score [ k - 1 ][ i ][ j - 1 ] + getScores( chr_j, '-', chr_k );
					if( tmp > max ){
						max = tmp;
						arrow = 5;
					}
				}
				if( j && i ){     // 6: xy-
					tmp = score [ k ][ i - 1 ][ j - 1 ] + getScores( chr_j, chr_i, '-' );
					if( tmp > max ){
						max = tmp;
						arrow = 6;
					}
				}
				if( k ){     // 1: --z
					tmp = score [ k - 1 ][ i ][ j ] + INDEL * 2;
					if( tmp > max ){
						max = tmp;
						arrow = 1;
					}
				}
				if( i ){     // 2:-y-
					tmp = score [ k ][ i - 1 ][ j ] + INDEL * 2;
					if( tmp > max ){
						max = tmp;
						arrow = 2;
					}
				}
				if( j ){     // 4: x--
					tmp = score [ k ][ i ][ j - 1 ] + INDEL * 2;
					if( tmp > max ){
						max = tmp;
						arrow = 4;
					}
				}
				tracing[ k ][ i ][ j ] = arrow; 
				if( arrow )     // if j || i || k
					score[ k ][ i ][ j ] = max;
				//cout << setw(5) << arrow; 
				//cout << setw(5) << max;
			}
			//cout << endl;
		}
		//cout << "end this layer: " << k << endl;
	}
	//cout << score[ len_z ][ len_y ][ len_x ];  
	backTracing( x, y, z, range, tracing, res );
	//printResult( res );
}
void getUpMidSpace( string & x, string & y, string & z, int range[3][2] , Space & space ){
	int & x_start = range[0][0];
	int & x_end = range[0][1];
	int & y_start = range[1][0];
	int & y_end = range[1][1];
	int & z_start = range[2][0];
	int & z_end = range[2][1];
	int len_x = x_end - x_start + 1;
	int len_y = y_end - y_start + 1;
	//int len_z = z_end - z_start + 1;   // check stdFillSpace and getUpMidspace get the same score when range is the same
	int len_z = ( z_end - z_start + 1 ) / 2;
	space[0][0] = 0;
	// upXArray and upY is used to save scores in the previous layer when update scores in 2D matrix (i.e, space ) from one layer to the next layer
	int * upXArray = new int[ len_x + 1 ];
	int upY;
	char chr_k, chr_i, chr_j;
	for( int k = 0; k <= len_z; ++k ){     // x: index j, y: index i ,  z: index k
		if( k )
			chr_k = z[ z_start + k - 2 ]; 
		for( int i = 0; i <= len_y; ++i ){
			if( i )
				chr_i = y[ y_start + i - 2 ];
			for( int j = 0; j <= len_x; ++j ){
				int max = INT_MIN;
				int tmp;
				if( j )
					chr_j =  x[ x_start + j - 2 ];
				if( j && i && k ){    // xyz, 111(2 ) = 7(10)
					max = upXArray[ j - 1 ] + getScores( chr_j, chr_i, chr_k ); 
					upXArray[ j - 1 ] = upY;   // upXArray[ j - 1 ] variable has been used to determine sapce[i][j], update to store new value 
				}	
				if( i && k ){     // 3: -yz
					tmp = upXArray[ j ] + getScores( '-',  chr_i, chr_k );
					if( tmp > max )
						max = tmp;
				}
				if( j && k ){     // 5: x-z
					tmp = upY + getScores( chr_j, '-', chr_k );
					if( tmp > max )
						max = tmp;
				}
				if( j && i ){     // 6: xy-
					tmp = space [ i - 1 ][ j - 1 ] + getScores( chr_j, chr_i, '-' );
					if( tmp > max )
						max = tmp;
				}
				if( k ){     // 1: --z
					upY = space[ i ][ j ];  // save old sapce[i][j] ,which will be upY relative to new space[ i ][ j + 1 ]
					tmp = space[ i ][ j ] + INDEL * 2;
					if( tmp > max )
						max = tmp;			
				}
				if( i ){     // 2:-y-
					tmp = space[ i - 1 ][ j ] + INDEL * 2;
					if( tmp > max )
						max = tmp;					
				}
				else if ( k )                // i == 0 && k != 0 , initialize upXarray 
					upXArray[ j ] = space[ 0 ][ j ];
				if( j ){     // 4: x--
					tmp = space[ i ][ j - 1 ] + INDEL * 2;
					if( tmp > max )
						max = tmp;					
				}			
				if( i || j || k ){
					space[ i ][ j ] = max;			
				}			
				//cout << setw(5) << space[ i ][ j ] ;			
			}
			if( k )  // before go to the next row , update the last element in upXarray
				upXArray[ len_x ] = space[ i ][ len_x ];	
			//cout << endl;
		}
		//cout << "end this layer: " << k << endl;
	}
	delete [] upXArray;
}
void getDownMidSpace( string & x, string & y, string & z, int range[3][2] , Space & space ){
	int & x_start = range[0][0];
	int & x_end = range[0][1];
	int & y_start = range[1][0];
	int & y_end = range[1][1];
	int & z_start = range[2][0];
	int & z_end = range[2][1];
	int len_x = x_end - x_start + 1;
	int len_y = y_end - y_start + 1;
	int len_z = z_end - z_start + 1;
	len_z = len_z - len_z / 2;
	space[ len_y ][ len_x ] = 0;
	int * downXArray = new int[ len_x + 1 ];
	int downY;
	char chr_k, chr_i, chr_j;
	for( int k = 0; k <= len_z; ++k ){     // x: index j, y: index i ,  z: index k
		if( k )
			chr_k = z[ z_end - k ]; 
		for( int i = len_y; i >= 0; --i ){
			if( i < len_y )
				chr_i = y[ y_start + i - 1 ];
			for( int j = len_x; j >=0 ; --j ){
				int max = INT_MIN;
				int tmp;
				if( j < len_x )
					chr_j =  x[ x_start + j - 1 ];
				if( j < len_x && i < len_y && k ){    // xyz, 111(2 ) = 7(10)
					max = downXArray[ j + 1 ] + getScores( chr_j, chr_i, chr_k ); 
					downXArray[ j + 1 ] = downY;   // downXArray[ j + 1 ] variable has been used to determine sapce[i][j], update to store new value 
				}	
				if( i < len_y && k ){     // 3: -yz
					tmp = downXArray[ j ] + getScores( '-',  chr_i, chr_k );
					if( tmp > max )
						max = tmp;
				}
				if( j < len_x && k ){     // 5: x-z
					tmp = downY + getScores( chr_j, '-', chr_k );
					if( tmp > max )
						max = tmp;
				}
				if( j < len_x && i < len_y ){     // 6: xy-
					tmp = space [ i + 1 ][ j + 1 ] + getScores( chr_j, chr_i, '-' );
					if( tmp > max )
						max = tmp;
				}
				if( k ){     // 1: --z
					downY = space[ i ][ j ];  // save old sapce[i][j] ,which will be upY relative to new space[ i ][ j - 1 ]
					tmp = space[ i ][ j ] + INDEL * 2;
					if( tmp > max )
						max = tmp;			
				}
				if( i < len_y){     // 2:-y-
					tmp = space[ i + 1 ][ j ] + INDEL * 2;
					if( tmp > max )
						max = tmp;					
				}
				else if ( k )                // i == len_y && k != 0 , initialize downXarray 
					downXArray[ j ] = space[ len_y ][ j ];
				if( j < len_x ){     // 4: x--
					tmp = space[ i ][ j + 1 ] + INDEL * 2;
					if( tmp > max )
						max = tmp;					
				}			
				if( i < len_y|| j < len_x || k ){
					space[ i ][ j ] = max;			
				}			
				//cout << setw(5) << max;			
			}
			if( k )  // before go to the next row ( --i ) , update the first element in downXarray
				downXArray[ 0 ] = space[ i ][ 0 ];	
			//cout << endl;
		}
		//cout << "end this layer: " << k << endl;
	}
	delete [] downXArray;
}
void fillLinearSpace ( string & x, string & y, string & z, int range[3][2] , Space & space1, Space & space2, vector<string> & res){
	int & x_start = range[0][0];
	int & x_end = range[0][1];
	int & y_start = range[1][0];
	int & y_end = range[1][1];
	int & z_start = range[2][0];
	int & z_end = range[2][1];
	int len_x = x_end - x_start + 1;
	int len_y = y_end - y_start + 1;
	int len_z = z_end - z_start + 1;
	if( len_x <= 1 || len_y <= 1 || len_z <= 1){
		stdFillSpace ( x, y, z, range, res);
		return;
	}
	getUpMidSpace( x, y, z, range, space1 );
	getDownMidSpace( x, y, z, range, space2 );
	vector<int> xyMidPoint(2);
	int maxScore = space1[ 0 ][ 0 ] + space2[ 0 ][ 0 ];
	for( int i = 0 ; i <= len_y;  ++i ){
		for( int j = 0; j <= len_x; ++j ){
			if( space1[i][j] + space2[i][j] > maxScore ){
				maxScore = space1[i][j] + space2[i][j];
				xyMidPoint[0] = i ; // row
				xyMidPoint[1] = j ; // column
			}
		}
	}
	int yMidPoint = y_start + xyMidPoint[0] - 1 ;
	int xMidPoint = x_start + xyMidPoint[1] - 1 ;
	if( len_z == (int) z.length() ){  //  the optimal alignment score
		score = maxScore;
	}
	int range2[3][2] = { { xMidPoint + 1, x_end }, 
						{ yMidPoint + 1 ,y_end }, 
						{ z_start + len_z / 2 , z_end } }; 
	int range1[3][2] = { { x_start, xMidPoint }, 
						{ y_start, yMidPoint }, 
						{ z_start, z_start + len_z / 2 - 1}  };
	fillLinearSpace ( x, y, z, range2, space1, space2, res );
	fillLinearSpace ( x, y, z, range1, space1, space2, res );
}
void readFile( const string & fileName, string & s ){
	ifstream fIn;
	fIn.open( fileName.c_str() );
	char c;
	while( fIn.get( c ) ){
		if( c == 'a' || c == 'c' || c == 'g' || c == 't' )
			s += c;
	}
	fIn.close();
}
struct compare {     
	bool operator()(const string& first, const string& second) {
		return first.size() < second.size();
	}
};
int main(){
	Timer t;
	t.start();
	double userTime;
	double realTime;
	ofstream fOut( "output_4.txt" );
	struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);
    fOut << "Maximal memory usage at the beginning of the program: " << r_usage.ru_maxrss << endl;
	/*string x = "cttaact";
	string y = "cgttact";
	string z = "cggatcat";*/
//reverse:
	/*string x = "tcaattc";
	string y = "tcattgc";
	string z = "tactaggc";*/

	/*string x = "atgcaat";
	string y = "agcatat";
	string z = "atcccattt";*/

	/*string x = "a";
	string y = "";
	string z = "";	*/

	vector<string> input(3);
/*	readFile( "NM_000558.txt", input[0] ); 
	readFile( "NM_008218.txt", input[1] );
	readFile( "NM_013096.txt", input[2] );  */

//	readFile( "NM_001030004.txt", input[0] );
//	readFile( "NM_178850.2.txt", input[1] );
//	readFile( "XM_514664.txt", input[2] ); 

//	readFile( "NM_000545.txt", input[0] ); 
//	readFile( "NM_000457.txt", input[1] );
//	readFile( "NM_008261.txt", input[2] ); 

	readFile( "NM_000492.txt", input[0] ); 
	readFile( "NM_021050.txt", input[1] );
	readFile( "NM_031506.txt", input[2] ); 

	sort( input.begin(), input.end(), compare() );  // to use the minumal sapce, let z be the longest and x be the shortest
	string &x = input[0];
	string &y = input[1];
	string &z = input[2];
	
	int range[3][2] = { { 1, (int)x.length() }, { 1, (int)y.length() }, { 1, (int)z.length()} }; 
	vector<string> res(3);
	//stdFillSpace ( x, y, z, range, res );
	Space space1( x.length(), y.length() );
	Space space2( x.length(), y.length() );
	fillLinearSpace( x, y, z, range, space1, space2, res );

	t.elapsedUserTime( userTime );
	t.elapsedWallclockTime( realTime );

	printResult( res, fOut );
	fOut << "The score of optimal alignment: " << score << endl;
	fOut << "The length of the alignment: " << res[0].length() << endl;
	fOut << "The number of perfectly matched columns: " << numPerfectMatch << endl;	
	getrusage(RUSAGE_SELF,&r_usage);
	fOut << "Maximal memory usage at the end of the program: " << r_usage.ru_maxrss << endl;
	fOut << "Real time: " << realTime << endl;
	fOut << "User time: " << userTime << endl;
	fOut.close();
}