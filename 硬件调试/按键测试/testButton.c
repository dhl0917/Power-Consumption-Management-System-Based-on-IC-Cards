#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>


#define f 		11.0592
#define BIT 	XBYTE[0X7FFF]


sbit sw1=P3^3;
sbit sw2=P3^5;
sbit sw3=P3^4;

sbit buz = P1^0;
sbit ledcontrol = P1^1;
sbit ic_card_in = P1^4;

void warning();
unsigned char data display_bit;



void main()
{
    
	while(1)
	{
	 	if(!sw1)
		{
		   display_bit=0xf0;
			BIT = display_bit;
		    
		}
		if(sw1)
		{
		   display_bit=0x00;
			BIT = display_bit;
		    
		}

		if(sw2)
		{
		   ledcontrol=1;
		}
		if(!sw2)
		{
		   ledcontrol=0;
		}
	  
	   if(!sw3)
		{
		   warning();
		}
		if(sw3)
		{
		   ;
		}

	}

}

void warning()
{	 
	int m;
    int j=0;
	 while(j<2)
	 {
	     j++;
		 buz=~buz;
		 for(m=0;m<5000;m++);
	 }
 }
