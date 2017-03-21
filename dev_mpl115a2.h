#ifndef __DEV_MPL115A2_H_INCLUDED__
#define __DEV_MPL115A2_H_INCLUDED__

//MPL115A2 Digital Barometer
class MPL115A2{
public:
  void Init();
  void Get();
  double Read( unsigned char data_type);
  void conv_test_exec();
private:
  double conv_double( unsigned int reg, unsigned int n);
  void conv_test(unsigned int test,unsigned int n);
  int fd;
  unsigned char DevAddr; // 0x60
  double padc;
  double tadc;
  double a0,b1,b2,c12,p_comp,p_hpa,temp;
};

#endif
