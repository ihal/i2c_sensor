#ifndef __SYS_DISPLAY_H_INCLUDED__
#define __SYS_DISPLAY_H_INCLUDED__
#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "sys_log.h"

class DISPLAY{
public:
  void Init();
  void Get();
  void Disp( const class LOG l);
private:
  struct tm* date;
  time_t timer;
};

#endif
