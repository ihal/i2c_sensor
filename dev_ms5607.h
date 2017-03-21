#ifndef __DEV_MS5607_H_INCLUDED__
#define __DEV_MS5607_H_INCLUDED__
//MS5607-02BA03 Barometric Pressure Sensor
class MS5607
{
public:
  void Init();
  void Get();
  double Read( unsigned char data_type);
private:
  static const unsigned char cmd_reset    = 0x1e;
  static const unsigned char cmd_adc_read = 0x00;
  static const unsigned char cmd_adc_conv = 0x40;
  static const unsigned char cmd_adc_d1   = 0x00;
  static const unsigned char cmd_adc_d2   = 0x10;
  static const unsigned char cmd_adc_256  = 0x00;
  static const unsigned char cmd_adc_512  = 0x02;
  static const unsigned char cmd_adc_1024 = 0x04;
  static const unsigned char cmd_adc_2048 = 0x06;
  static const unsigned char cmd_adc_4096 = 0x08;
  static const unsigned char cmd_prom_rd  = 0xa0;
  int fd;
  unsigned char DevAddr; // 0x77
  unsigned int c0,c1,c2,c3,c4,c5,c6,c7;//16bit
  unsigned int d1,d2;//24bit
  int dt,temp;
  int64_t off,sens;
  int p;
  unsigned int swap16(unsigned int reg);
  void debug();
  void debug1();
};
#endif
