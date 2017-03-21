#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "dev_ms5607.h"

//using namespace std;
//MS5607-02BA03 Barometric Pressure Sensor
unsigned int MS5607::swap16(unsigned int reg)
{
  reg=reg&0x0000ffff;
  reg=(((reg<<8)|(reg>>8))&0x0000ffff);
  return(reg);
}
void MS5607::debug1()
{
//debug data;
  c1=46372;
  c2=43981;
  c3=29059;
  c4=27842;
  c5=31553;
  c6=28165;
  d1=6465444;
  d2=8077636;
}

void MS5607::debug()
{
  printf("c1   c2    c3    c4    c5    c6\n");
  printf("%05d %05d %05d %05d %05d %05d\n",
    c1,c2,c3,c4,c5,c6);

  printf("%04x %016x\n",0,c0);
  printf("%04x %016x\n",1,c1);
  printf("%04x %016x\n",2,c2);
  printf("%04x %016x\n",3,c3);
  printf("%04x %016x\n",4,c4);
  printf("%04x %016x\n",5,c5);
  printf("%04x %016x\n",6,c6);
  printf("%04x %016x\n",7,c7);

  printf("d1        d2\n");
  printf("%08d  %08d\n",d1,d2);

  printf("dt        temp\n");
  printf("%08d  %08d\n",dt,temp);

  printf("off       sens    p\n");
  printf("%10lld  %10lld  %08d\n",off,sens,p);

}

void MS5607::Init()
{
  DevAddr=0x77;
  fd= wiringPiI2CSetup(DevAddr);
  if(fd<0)
    {
      std::cerr <<"センサー設定エラー"<<std::endl;
    }

  wiringPiI2CWrite(fd, cmd_reset); //Reset
  usleep(1000000);

  unsigned int reg;
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+0);
  c0=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+2);
  c1=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+4);
  c2=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+6);
  c3=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+8);
  c4=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+10);
  c5=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+12);
  c6=swap16(reg);
  reg=wiringPiI2CReadReg16(fd,cmd_prom_rd+14);
  c7=swap16(reg);
}
void MS5607::Get()
{
  unsigned int reg;
  unsigned char buff[4];

  wiringPiI2CWrite(fd, cmd_adc_conv|cmd_adc_d1|cmd_adc_4096); //Convert
  usleep(100000);
  wiringPiI2CWrite(fd, cmd_adc_read); //
  read(fd,buff,3);
  reg=0;
  reg=reg |(buff[0]<<16);
  reg=reg |(buff[1]<<8);
  reg=reg |(buff[2]<<0);
  d1=reg; 

  wiringPiI2CWrite(fd, cmd_adc_conv|cmd_adc_d2|cmd_adc_4096); //Convert
  usleep(100000);

  wiringPiI2CWrite(fd, cmd_adc_read); //
  read(fd,buff,3);
  reg=0;
  reg=reg |(buff[0]<<16);
  reg=reg |(buff[1]<<8);
  reg=reg |(buff[2]<<0);
  d2=reg; 

  //  debug1();
  dt=(int)(d2-(c5<<8));
  temp=2000+((dt*c6)>>23);
  int64_t x1,x2,x3;
  x1=(int64_t)c2<<17;
  x2=(c4*dt);
  x3=x2>>6;
  off=x1+x3;
  //  printf("x1=%lld x2=%lld x3=%lld off=%lld\n",x1,x2,x3,off);
  //off=(c2<<17)+((c4*dt)>>6);
  sens=(c1<<16)+((c3*dt)>>7);
  x1=d1*sens>>21;
  p=(int)((static_cast<int64_t>(x1)-static_cast<int64_t>(off))>>15);
  //  debug();
}
double MS5607::Read(unsigned char data_type)
{
  if(data_type==0) return ((double)temp/100);
  if(data_type==1) return ((double)p/100);
  return((double)p/100);
}
