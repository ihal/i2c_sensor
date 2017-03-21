#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "dev_hdc1000.h"

void HDC1000::Init()
{
// デバイスアドレス 
// i2cdetect -y 1 で調べられる
  DevAddr=0x40;
// 温度取得ポインタとConfigポインタのアドレス指定。
// 基本的にこのままでOK。今回は温度と湿度を同時取得するので
// 温度側だけでOK
  tempp=0x00;
  confp=0x02;
// デバイスの設定
// 2バイトの設定データを1バイトに分けて設定。
// 1＝上位ビット側 2＝下位ビット側
// 0x1234という設定データなら 1＝0x12 2＝0x34と設定
  devconf1=0x10;
  devconf2=0x00;

  unsigned char ConfData[3];
  ConfData[0]= confp;
  ConfData[1]= devconf1;
  ConfData[2]= devconf2;
  fd=wiringPiI2CSetup(DevAddr);
  /*	
  wiringPiSetupGpio();
  pinMode(RDPin,INPUT);
  */
  if(write(fd,ConfData,sizeof(ConfData))<0)
    {
      std::cerr <<"センサー設定エラー"<<std::endl;
    }
  //  return fd;
}
void HDC1000::Get()
{
  unsigned char GetData[1];
  unsigned char ReData[4];

  GetData[0]=tempp;
  // センサーに温度測定要求
  if(write(fd,GetData,1)<0)
    {
      std::cerr <<"データ要求エラー"<<std::endl;
    }
  // センサーが測定→変換を完了させるのを待つ
  //	while((digitalRead(RDPin))==1);
  usleep(500000);
  // データ取得
  if(read(fd,ReData,4)<0)
    {
      std::cerr <<"データ受信エラー"<<std::endl;
    }
  int conv;
  conv=(ReData[0]<<8)|(ReData[1]);
  // 戻ってきたデータをくっつけて返す
  temp= ( (double)(conv)/65536.000) * 165.000 - 40.000;
  conv=(ReData[2]<<8)|(ReData[3]);
  humi= ( (double)(conv)/65536.000) * 100.000;
}
double HDC1000::Read(unsigned char data_type)
{
  if(data_type==0) return (temp);
  if(data_type==1) return (humi);
  return(temp);
}
