
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
using namespace std;
int gcd(int a, int b){
	if( a == b)
		return a;
	while( a != 0 )
		if ( a > b )
			a = a % b;
		else
		 swap( a, b );
	return b;
}
bool diophantine( int ori_a, int ori_b, int ori_c, int &x, int &y){
	int a = abs(ori_a);
	int b = abs(ori_b);
	int c = abs(ori_c);	
	int valueGcd = gcd(a,b);
	if( c % valueGcd != 0 )
		return false;
	a /= valueGcd;
	b /= valueGcd;
	c /= valueGcd;
	if( a % b == 0 ){
		x = 0;
		y = c / b * abs ( ori_b * ori_c) / ori_b / ori_c;
		return true;
	}
	int r = a % b;
	int q = a / b;
	int u = q * x + y;
	int v = x;
	a = b;
	if( diophantine ( b, r, c, u, v ) ){
		x = v ;
		y = ( u - q * x ) * (abs ( ori_b * ori_c) / ori_b / ori_c );
		x = x * abs(ori_a * ori_c) / ori_a / ori_c ;
	}
	return true;
}
int main(){
	int a, b, c ;
	int x = 0, y = 0;
	string input, output;
	cout << "What is the name of the input file?" << endl;
	getline(cin, input);
	cout << "What is the name of the output file?" << endl;
	getline(cin, output);
	ifstream fIn(&input[0]);
	ofstream fOut(&output[0]);
	bool hasOutput = false;
	while( 	fIn >> a >> b >> c ){
		if ( hasOutput )
			fOut << endl;
		hasOutput = true;		
		//if( !a || !b )
		//	return 0;
		if( diophantine( a, b, c, x, y) ) 
			fOut << x << " " << y ;	
		else
			fOut << "No Solution!" ;
	}
	fIn.close();
	fOut.close();
	return 0;
}