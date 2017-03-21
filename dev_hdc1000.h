#ifndef __DEV_HDC1000_H_INCLUDED__
#define __DEV_HDC1000_H_INCLUDED__

//HDC1000 Humidity and Temperature Digital Sensor Class
class HDC1000{
public:
  void Init(); // 
  void Get();
  double Read( unsigned char data_type);
private:
  int fd;
// デバイスアドレス 
// i2cdetect -y 1 で調べられる
  unsigned char DevAddr;
// 温度取得ポインタとConfigポインタのアドレス指定。
// 基本的にこのままでOK。今回は温度と湿度を同時取得するので
// 温度側だけでOK
  unsigned char tempp;
  unsigned char confp;
// デバイスの設定
// 2バイトの設定データを1バイトに分けて設定。
// 1＝上位ビット側 2＝下位ビット側
// 0x1234という設定データなら 1＝0x12 2＝0x34と設定
  unsigned char devconf1;
  unsigned char devconf2;
  //
  double temp;
  double humi;

};
#endif
