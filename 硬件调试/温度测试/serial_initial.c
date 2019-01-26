#include<reg52.h>
#include<stdio.h>
void serial_initial(void)
{
SCON=0X52;
TMOD=0X20;
TH1=0XF3;
TR1=1;
}
   