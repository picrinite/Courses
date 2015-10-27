#include "Date.h"
#include <cstring>
bool Date::isLeap( unsigned i) const{
	return i%4==0 && i%100!=0 || i%400==0; //&& 优先级大于 ||
}
unsigned Date::daysPerMonth( unsigned m, unsigned y ) const{
	switch(m){
		case (2): 
			if(isLeap(y))
				return 29;
			else return 28;
		case (4): return 30;
		case (6): return 30;
		case (9): return 30;
		case (11): return 30;
		default: return 31;
	}
}
string Date::name( unsigned m ) const{
	switch(m){
		case (1): return "January";
		case (2): return "February";
		case (3): return "March";
		case (4): return "April";
		case (5): return "May";
		case (6): return "June";
		case (7): return "July";
		case (8): return "August";
		case (9): return "September";
		case (10): return "October";
		case (11): return "November";
		default: return "December";
	}
}
unsigned Date::number( const string & mn ) const{
	if(strcmp(mn.c_str(),"January")==0|| strcmp(mn.c_str(),"january")==0)
		return 1;
	else if(strcmp(mn.c_str(),"February")==0 || strcmp(mn.c_str(),"february")==0 )
		return 2;
	else if(strcmp(mn.c_str(),"March")==0 || strcmp(mn.c_str(),"march")==0)
		return 3;
	else if(strcmp(mn.c_str(),"April")==0 || strcmp(mn.c_str(),"april")==0)
		return 4;
	else if(strcmp(mn.c_str(),"May")==0 || strcmp(mn.c_str(),"may")==0)
		return 5;
	else if(strcmp(mn.c_str(),"June")==0 || strcmp(mn.c_str(),"june")==0)
		return 6;
	else if(strcmp(mn.c_str(),"July")==0 || strcmp(mn.c_str(),"july")==0)
		return 7;
	else if(strcmp(mn.c_str(),"August")==0 || strcmp(mn.c_str(),"august")==0)
		return 8;
	else if(strcmp(mn.c_str(),"September")==0 || strcmp(mn.c_str(),"september")==0)
		return 9;
	else if(strcmp(mn.c_str(),"October")==0 || strcmp(mn.c_str(),"october")==0)
		return 10;
	else if(strcmp(mn.c_str(),"November")==0 || strcmp(mn.c_str(),"november")==0)
		return 11;
	else if(strcmp(mn.c_str(),"December")==0 || strcmp(mn.c_str(),"december")==0)
		return 12;
	else return 0;
}
Date::Date(){
	day=1;
	month=1;
	year=2000;
}
Date::Date( unsigned m, unsigned d, unsigned y ){
	bool validity=true;
	if(m==0){
		m=1;
		validity=false;
	}
	else if(m>12){
		m=12;
		validity=false;
	}
	if(d==0){
		d=1;
		validity=false;
	}
	else if(d>daysPerMonth(m,y)){
		d=daysPerMonth(m,y);
		validity=false;
	}
	if(!validity)
		cout<<"Invalid date values: Date corrected to "<<m<<"/"<<d<<"/"<<y<<"."<<endl;
	month=m;
	day=d;
	year=y;
}
Date::Date( const string &mn, unsigned d, unsigned y ){
	month=number(mn);
	bool validity=true;
	if(!month){
		month=1;
		day=1;
		year=2000;
		cout<<"Invalid month name: the Date was set to 1/1/2000"<<endl;
	}
	else {
		if(d==0){
			d=1;
			validity=false;
		}
		else if(d>daysPerMonth(month,y)){
			d=daysPerMonth(month,y);
			validity=false;
		}
		if(!validity)
			cout<<"Invalid date values: Date corrected to "<<month<<"/"<<d<<"/"<<y<<"."<<endl;
		day=d;
		year=y;
	}
}
void Date::printNumeric( ) const{
	cout<<month<<"/"<<day<<"/"<<year;
}
void Date::printAlpha( ) const{
	cout<<name(month).c_str()<<" "<<day<<", "<<year;
}
Date Date::addDays( int a) const{
	int dayCurYear=0;
	for(int i=1	;i<month;++i){
		 dayCurYear+=daysPerMonth(i,year);
	}
	dayCurYear+=day;
	int y=year;
	int days;
	int iMonth;
	if(a>=0){
		days=dayCurYear;  //days save the totals days which including dayCurYear
		days+=a;
		while(1){
			if(days>366 && isLeap(y)){
				days-=366;
				y++;
			}
			else if(days>365 && !isLeap(y) ){
				days-=365;
				y++;
			}
			else {
				for(iMonth=1;iMonth<12;++iMonth){
					if(days<=daysPerMonth(iMonth,y))
						break;
					days-=daysPerMonth(iMonth,y);
				}
				break;
			}
		}
		return Date(iMonth,days,y);
	}
	else{
		if(isLeap(y))
			days=366-dayCurYear-a;
		else
			days=365-dayCurYear-a;
		while(1){
			if(days>=366 && isLeap(y)){
				days-=366;
				y--;
			}
			else if(days>=365 && !isLeap(y)){
				days-=365;
				y--;
			}
			else{
				for(iMonth=12;iMonth>1;--iMonth){
					if(days<daysPerMonth(iMonth,y)){
						days=daysPerMonth(iMonth,y)-days;
						break;
					}
					days-=daysPerMonth(iMonth,y);
				}
				break;
			}
		}
		return Date(iMonth,days,y);
	}
}

