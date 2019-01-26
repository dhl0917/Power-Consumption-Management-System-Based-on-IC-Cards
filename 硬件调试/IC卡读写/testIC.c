#include<reg52.h>
#include<stdio.h>
#include<intrins.h>
#include<absacc.h>

extern serial_initial();
sbit SCL_IC_CARD=P1^5;
sbit SDA_IC_CARD=P1^6;
sbit WP_IC_CARD=P1^7;



bdata char com_data;
sbit mos_bit=com_data^7;
sbit low_bit=com_data^0;

void delay(int n);
unsigned char rd_24c01(char a);
void wr_24c01(char a,char b);
void main()
{
	unsigned char i;
	serial_initial();
	WP_IC_CARD=1;
				
	//write
			wr_24c01(2,0x56);
			delay(250);
	
			wr_24c01(1,0x34);
			delay(250);
	
			wr_24c01(0,0x12);
			delay(250);
	

	
	//read
	printf("\nRead the data: ");
	for(i=0;i<=2;i++)
	{
			printf("%bX",rd_24c01(i));
			delay(250);
	}
	
}
void start()
{
	SDA_IC_CARD=1;
	SCL_IC_CARD=1;
	SDA_IC_CARD=0;
	SCL_IC_CARD=0;
}
void stop()
{
	SDA_IC_CARD=0;
	SCL_IC_CARD=1;
	SDA_IC_CARD=1;
}
void ack()
{
	SCL_IC_CARD=1;
	SCL_IC_CARD=0;
}
void shift8(char a)
{
	data unsigned char i;
	com_data=a;
	for(i=0;i<8;i++)
	{
		SDA_IC_CARD=mos_bit;
		SCL_IC_CARD=1;
		SCL_IC_CARD=0;
		com_data=com_data*2;
	}
}

unsigned char rd_24c01(char a)
{
	data unsigned char i,command;
	SDA_IC_CARD=1;
	SCL_IC_CARD=0;
	start();
	command=160;
	shift8(160);
	ack();
	shift8(a);
	ack();
	start();
	command=161;
	shift8(161);
	ack();
	SDA_IC_CARD=1;
	for(i=0;i<8;i++)
	{
		com_data=com_data*2;
		SCL_IC_CARD=1;
		low_bit=SDA_IC_CARD;
		SCL_IC_CARD=0;
	}
	stop();
	return(com_data);
}

void wr_24c01(char a,char b)
{
	data unsigned char command;
	WP_IC_CARD=0;
	_nop_();
	SDA_IC_CARD=1;
	SCL_IC_CARD=0;
	start();
	command=160;
	shift8(160);
	ack();
	shift8(a);
	ack();
	shift8(b);
	ack();
	stop();
	_nop_();
	WP_IC_CARD=1;
}
void delay(int n)
{
	int i;
	for(i=1;i<=n;i++){;}
}