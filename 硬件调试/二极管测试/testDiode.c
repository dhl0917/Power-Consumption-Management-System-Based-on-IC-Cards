#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>


#define f 		11.0592
#define BIT 	XBYTE[0X7FFF]

sbit buz = P1^0;
sbit ledcontrol = P1^1;
sbit ic_card_in = P1^4;

void warning();
unsigned char data display_bit;

void delay(long n);

void main()
{
    
	while(1)
	{
	 	if(ic_card_in)
		{
		   ledcontrol=0;
		   display_bit=0x00;
			BIT = display_bit;
		    warning();
		}
		else if(!ic_card_in) 
		{
			ledcontrol=1; 
			display_bit=0xf0;
			BIT = display_bit;
		}

	}

}


void delay (long n)
{
	long i;
	for (i=1;i<=n;i++)
	{;}
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
//break;
			/*
					   */