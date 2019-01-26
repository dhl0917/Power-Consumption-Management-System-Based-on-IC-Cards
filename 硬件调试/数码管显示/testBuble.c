#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>

#define f 		11.0592
#define SEGMENT XBYTE[0XBFFF]
#define BIT 	XBYTE[0X7FFF]

unsigned char code seg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,
								0x07,0x7F,0x67};
unsigned char code seg_code_fra[]={0xbf,0x86,0xdb,0xcf,0xe6,0xeD,0xfD,
								0x87,0xff,0xe7};
unsigned char data display_bit;	    	//ledÎ»Âë
unsigned char data display_buffer;		//led¶ÎÂë
void delay(long n);

void main()
{
	
	int i=0,j=0;
	
	while(1)
	{
		display_buffer=seg_code_fra[8];
		SEGMENT=display_buffer;
		display_bit=0x0f;
		BIT= display_bit;
		delay(50000);
		
		 for(i=0;i<10;i++)
		{
			display_buffer=seg_code_fra[i];
			SEGMENT=display_buffer;
			delay(10000);
		}
		   	
		for(j=0;j<10;j++)
		{
		 	display_buffer=seg_code[j];
			SEGMENT=display_buffer;
			display_bit=0x08;
			BIT= display_bit;

			for(i=0;i<4;i++)
		 	{
				delay(3000);
				display_bit=display_bit>>1;
				BIT= display_bit;

		 	}
		}	
		

	}
}



void delay(long n)
{
long i;
for (i=1;i<=n;i++){;}
}