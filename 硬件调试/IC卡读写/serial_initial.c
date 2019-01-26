#include <reg52.h>
#include <stdio.h>
void serial_initial(void)
{
SCON=0x52;
TMOD=0x20;
TH1=0xF3;
TR1=1;
}