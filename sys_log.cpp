#include "sys_log.h"
using namespace std;

string LOG::Compose() {
	ostringstream ost;
	
	ost << "'"
	    << setfill('0')
	    << setw(4)
	    << year
	    << "-"
	    << setw(2)
	    << month
	    << "-"
	    << setw(2)
	    << day
	    << " "
	    << setw(2)
	    << hour
	    << ":"
        << setw(2)
	    << minute
	    << ":"
	    << setw(2)
	    << second
	    << "+9" << "', "
	    << fixed<<setprecision(0)
		<< temp1*100 << ","
	    << humi1*100 << ","
	    << (pres2-1000)*100 << ","
	    << temp2*100 << ","
	    << (pres3-1000)*100 << ","
	    << temp3*100
	    ;
	return ost.str();
}
void LOG::Write( 
                 double _temp1,double _humi1,
                 double _pres2,double _temp2,
                 double _pres3,double _temp3
               ) {
	temp1 = _temp1;
	humi1 = _humi1;
    pres2 = _pres2;
	temp2 = _temp2;
	pres3 = _pres3;
	temp3 = _temp3;
}

void LOG::GetTime() {
  //現在時刻の取得
  time(&timer);
  date   = localtime(&timer);

  year   = date->tm_year+1900;
  month  = date->tm_mon+1;
  day    = date->tm_mday;
  hour   = date->tm_hour;
  minute = date->tm_min;
  second = date->tm_sec;
  
}
