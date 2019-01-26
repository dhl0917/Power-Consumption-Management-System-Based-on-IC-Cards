#include <at89x52.h>
#include <stdio.h>
#include <absacc.h>
#include <intrins.h>
#define fosc 		11.0592				//单位为MHz,时钟频率决定后，定时器最大定时间为
										//65536*(12/fosc),约为71mS.TN<=65536*12/fosc
										//(TN小于等于60397，即EBEDH).定时器1定时时间
										//(Tx<=65536*12/fosc=60397us)

#define adj	-17							//用以调整波特率，是具体情况取值
extern bit RIx,TIx;						//,TIME_FLAG,error;
unsigned char thx,tlx;
unsigned bak_TCON,bak_TMOD,bak_IE,bak_IP,bak_TH0,bak_TL0;
void UART_initial(unsigned int x);
void send(unsigned char x);
signed int receiv();
void restore();
void wait_bit();


sbit TXD1=P1^3;
sbit RXD1=P1^2;
sbit test=P2^0;

void UART_initial(unsigned int x)
{
	bak_TCON=TCON;
	bak_TMOD=TMOD;
	bak_IE=IE;
	bak_IP=IP;
	bak_TH0=TH0;
	bak_TL0=TL0;
	TR0=0;								//定时/计数器0停止工作
	TMOD=(TMOD & 0XF0) | 0x01;			//不影响此前对TMOD的操作，定时器0为16为定时模式
	x=-(((1000000/x)/2)*fosc/12+adj);	//取波特率二分之一周期作为定时时间
	TH0=x/256;
	TL0=x%256;
	thx=TH0;
	tlx=TL0;
	RXD1=1;								//设置为输入
}
void send(unsigned char x)
{		unsigned char i;
		TXD1=0;				  			//起始位 
		wait_bit();						//起始位的定时时间到
		for(i=0;i<=7;i++)
		  {
			TXD1=x & 0x01;
			x=x>>1;
			wait_bit();					//一位的定时时间到
		  }
		TXD1=1;				  			//停止位
		wait_bit();						//停止位的定时时间到
		TIx=1;							//置发送完成标志
}
void wait_bit()
{
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//第一次定时时间到
		TR0=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//第二次定时时间到
		TR0=0;
}
signed int receiv()
{		unsigned char temp,i,x;
//起始位检测开始---------------------------------------------------------
		while(RXD1);					//等待起始位
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//第一次定时时间到
		TR0=0;
		test=1;
		temp=(char)RXD1;
		test=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//起始位时间到
		TR0=0;						
		if(temp==1) return(-1);			//不是合格的起始位，返回-1
//起始位检测结束---------------------------------------------------------
//8位数据接收开始--------------------------------------------------------
		for(i=0;i<=7;i++)
		{
			TH0=thx;TL0=tlx;TF0=0;TR0=1;
			while(!TF0);					//第一次定时时间到
			TR0=0;
			test=1;
			temp=RXD1;
			test=0;
			x=x>>1;
			if(RXD1) x=x | 0x80;
			TH0=thx;TL0=tlx;TF0=0;TR0=1;
			while(!TF0);					//一个完整位时间到
			TR0=0;		  
		}
//8位数据接收完毕--------------------------------------------------------
//停止位检测开始---------------------------------------------------------
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//第一次定时时间到
		TR0=0;
		test=1;
		temp=RXD1;						//在停止位四分之二时间处读引脚
		test=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//停止位时间到
		TR0=0;			   				//
		if(temp!=1) return(-1);
//停止位检测完毕---------------------------------------------------------
		RIx=1;						   	//置接收就绪标志
		return(x);
}
void restore()
{
	TCON=bak_TCON;
	TMOD=bak_TMOD;
	IE=bak_IE;
	IP=bak_IP;
	TH0=bak_TH0;
	TL0=bak_TL0;
}
