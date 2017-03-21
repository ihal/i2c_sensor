
#include "sys_display.h"
#include "sys_log.h"

void DISPLAY::Init() {
}
void DISPLAY::Get() {
  //現在時刻の取得
  time(&timer);
  date=localtime(&timer);
}
void DISPLAY::Disp(const class LOG l) {
  // 表示
  printf("%.24s 気温：%.2f℃  湿度：%.2f％ 気圧： (1) %.2fhp %.2fdeg (2) %.2fmbar %.2fdeg\n",
    asctime(l.date),
    l.temp1,//Temp
    l.humi1,//Humi
    l.pres2,
    l.temp2,
    l.pres3,
    l.temp3
  );
}


