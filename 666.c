#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>


#define f 		11.0592
#define BUFF_SIZE  17
#define SEGMENT XBYTE[0XBFFF]
#define BIT 	XBYTE[0X7FFF]

unsigned char code seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,
								0x07,0x7F,0x67};
unsigned char d[6] _at_ 0x50;
unsigned char data display_bit;	    	//ledÎ»Âë
unsigned char data display_buffer;		//led¶ÎÂë
void delay(long n);


sbit sw1=P3^3;
sbit sw2=P3^5;
sbit sw3=P3^4;

sbit buz = P1^0;
sbit ledcontrol = P1^1;
sbit ic_card_in = P1^4;

void warning();
unsigned char data display_bit;
unsigned char data display_buffer;



void main()
{
    
	while(1)
	{
	 	if((!sw1)&&(sw2)&&(sw3))
		{
		   display_bit=0xff;
			BIT = display_bit;
		    display_buffer=seg_code[8];
			 SEGMENT=display_buffer;
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

		if((!sw1)&&(!sw2)&&(!sw3))
		{
			 display_bit=0xff;
			 BIT= display_bit;
			 display_buffer=seg_code[6];
			 SEGMENT=display_buffer;
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
 void delay(long n)
{
long i;
for (i=1;i<=n;i++){;}
}
