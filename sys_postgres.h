#ifndef __SYS_POSTGRES_H_INCLUDED__
#define __SYS_POSTGRES_H_INCLUDED__

#include <iostream>
#include <string>
#include <memory>
#include <pqxx/pqxx>

#include "sys_log.h"

//using namespace std;
//using namespace pqxx;

class DB{
public:
  void Connect();
  void Reconnect();
  void Disconnect();
  void Insert(class LOG l);
private:
  pqxx::connection* C;
};
#endif

