#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "dev_mpl115a2.h"

//MPL115A2 Digital Barometer
double MPL115A2::conv_double( unsigned int reg, unsigned int n)
{
  union 
  {
    double d;
    uint64_t i;
    struct
    {
      uint64_t frac :52;
      unsigned int exp :11;
      unsigned int sign :1;
    } b;
  }u;
  unsigned int s,m;
  u.i=0;
  reg=(((reg<<8)|(reg>>8))&0x0000ffff);
  if(reg==0)
    {
      u.d=0;
    }
  else 
    {
      if(((reg>>15)&0x01)==0)
        {
          u.b.sign=0;
        }
      else
        {
          u.b.sign=1;
          reg=((reg^(-1))+1)&0x0000ffff;
        }
      m=0x8000;
      for(s=0;s<15;s++)
	{
	  if(((m>>s)&reg) != 0) break;
	}
      u.b.frac = (uint64_t)((reg<<s)&0x7fff)<<(52-15);
      u.b.exp  = 0x03ff+n-s;
    }
  return(u.d);
}
void MPL115A2::conv_test(unsigned int reg,unsigned int n)
{
  printf("--------------\n");
  printf("reg=%08x n=%d\n",reg,n);
  reg=(((reg<<8)|(reg>>8))&0x0000ffff);
  printf("reg=%08x n=%d\n",reg,n);
  double d;
  d=conv_double(reg,n);
  printf("dob=%g \n",d);
  printf("dob=%15.10f \n",d);
  printf("dob=%15.10e \n",d);
}
void MPL115A2::conv_test_exec()
{
  conv_test(0x0000,12);
  conv_test(0x8000,12);
  conv_test(0xffff,12);

  conv_test(0x3ece,12);
  conv_test(0xb3f9,2);
  conv_test(0xc517,1);
  conv_test(0x33c8,-9);

  conv_test(0x6680>>1,10);//unsigned なので>>1
  conv_test(0x7ec0>>1,10);
}
void MPL115A2::Init()
{
  DevAddr=0x60;
  fd= wiringPiI2CSetup(DevAddr);
  if(fd<0)
    {
      std::cerr <<"センサー設定エラー"<<std::endl;
    }

  unsigned int reg;
  reg=wiringPiI2CReadReg16(fd,0x04);//a0
  a0=conv_double(reg,12);

  reg=wiringPiI2CReadReg16(fd,0x06);//b1
  b1=conv_double(reg,2);

  reg=wiringPiI2CReadReg16(fd,0x08);//b2
  b2=conv_double(reg,1);

  reg=wiringPiI2CReadReg16(fd,0x0a);//c12
  c12=conv_double(reg,-9);
}
void MPL115A2::Get()
{

  /*
  //  printf("MPL115A2:StartConv1\n");
  wiringPiI2CWrite(fd, 0x12); //Start Conversions
  wiringPiI2CWrite(fd, 0x00); //
  usleep(100000);
  //  printf("MPL115A2:StartConv2\n");
  wiringPiI2CWrite(fd, 0x12);
  wiringPiI2CWrite(fd, 0x01);
  usleep(100000);
  */

  
  wiringPiI2CWriteReg8(fd,0x12,0);//Start Conversions
  usleep(100000);

  unsigned int reg;
  reg=wiringPiI2CReadReg16(fd,0x00);
  //  printf("MPL115A2:padc %x\n",reg);
  padc=conv_double(reg>>1,10);//unsigned なので >>1
  reg=wiringPiI2CReadReg16(fd,0x02);
  //  printf("MPL115A2:tadc %x\n",reg);
  tadc=conv_double(reg>>1,10);
  //  printf("MPL115A2:padc %.4f tadc %.4f\n",padc,tadc);

  p_comp = a0 + (b1 + c12 * tadc) * padc + b2 * tadc;
  p_hpa = ((65.0/1023.0) * p_comp + 50) * 10;
  temp=(tadc-498.0)/(-5.35)+25.0;
  //  printf("MPL115A2:temp %.4f p_hpa %.4f\n",temp,p_hpa);

}
double MPL115A2::Read(unsigned char data_type)
{
  if(data_type==0) return (temp);
  if(data_type==1) return (p_hpa);
  return(p_hpa);
}
