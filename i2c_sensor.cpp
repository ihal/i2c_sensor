#include <iostream>
#include <thread>
#include <signal.h>

#include "dev_hdc1000.h"
#include "dev_mpl115a2.h"
#include "dev_ms5607.h"

#include "sys_governor.h"
#include "sys_log.h"
#include "sys_display.h"
#include "sys_postgres.h"


// using namespace std;

static int sig =0;
void sig_handler(int signo)
{
  sig=1;
}

int main()
{
  HDC1000 hdc1000_1;
  MPL115A2 mpl115a2_1;
  MS5607 ms5607_1;


  DISPLAY d;
  LOG     l;
  DB      db;
  
  

  hdc1000_1.Init();
  mpl115a2_1.Init();
  ms5607_1.Init();

  std::cout << "Hello\nWorld!\n";

  db.Connect();

  if( signal(SIGINT, sig_handler) == SIG_ERR) {
    std::cerr << "sig_handler fail."<<std::endl;
  }


  while(1) {
    std::thread th0(governor);
    //
    std::thread th1([&]{hdc1000_1.Get();});
    std::thread th2([&]{mpl115a2_1.Get();});
    std::thread th3([&]{ms5607_1.Get();});
    //
    l.GetTime();//現在時刻の取得
    //
    th1.join();
    th2.join();
    th3.join();
    //
    l.Write(
      hdc1000_1.Read(0),//Temp
      hdc1000_1.Read(1),//Humi
      mpl115a2_1.Read(1),
      mpl115a2_1.Read(0),
      ms5607_1.Read(1),//
      ms5607_1.Read(0) //Temp
    );
    // 表示
    d.Disp(l);
    // DBインサート
    db.Insert(l);
    //  Wait
    th0.join();
    //  Terminate
    if(sig) {
      std::cout <<"Recieve SIGINT\n";
      db.Disconnect();
      std::cout <<std::flush;
      break;
    }
  }
}
