#ifndef __SYS_LOG_H_INCLUDED__
#define __SYS_LOG_H_INCLUDED__
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "sys_display.h"
class LOG{
  public:
    friend class DISPLAY;
    void GetTime();
    void Write(
      double _temp1,
      double _humi1,
      double _pres2,
      double _temp2,
      double _pres3,
      double _temp3
    );
    std::string Compose();
    double temp1;
    double humi1;
    double pres2;
    double temp2;
    double pres3;
    double temp3;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    
  private:
    struct tm* date;
    time_t timer;
};
#endif
