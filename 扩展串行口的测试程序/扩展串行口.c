#include <at89x52.h>
#include <stdio.h>
#include <absacc.h>
#include <intrins.h>
#define fosc 		11.0592				//��λΪMHz,ʱ��Ƶ�ʾ����󣬶�ʱ�����ʱ��Ϊ
										//65536*(12/fosc),ԼΪ71mS.TN<=65536*12/fosc
										//(TNС�ڵ���60397����EBEDH).��ʱ��1��ʱʱ��
										//(Tx<=65536*12/fosc=60397us)

#define adj	-17							//���Ե��������ʣ��Ǿ������ȡֵ
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
	TR0=0;								//��ʱ/������0ֹͣ����
	TMOD=(TMOD & 0XF0) | 0x01;			//��Ӱ���ǰ��TMOD�Ĳ�������ʱ��0Ϊ16Ϊ��ʱģʽ
	x=-(((1000000/x)/2)*fosc/12+adj);	//ȡ�����ʶ���֮һ������Ϊ��ʱʱ��
	TH0=x/256;
	TL0=x%256;
	thx=TH0;
	tlx=TL0;
	RXD1=1;								//����Ϊ����
}
void send(unsigned char x)
{		unsigned char i;
		TXD1=0;				  			//��ʼλ 
		wait_bit();						//��ʼλ�Ķ�ʱʱ�䵽
		for(i=0;i<=7;i++)
		  {
			TXD1=x & 0x01;
			x=x>>1;
			wait_bit();					//һλ�Ķ�ʱʱ�䵽
		  }
		TXD1=1;				  			//ֹͣλ
		wait_bit();						//ֹͣλ�Ķ�ʱʱ�䵽
		TIx=1;							//�÷�����ɱ�־
}
void wait_bit()
{
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//��һ�ζ�ʱʱ�䵽
		TR0=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//�ڶ��ζ�ʱʱ�䵽
		TR0=0;
}
signed int receiv()
{		unsigned char temp,i,x;
//��ʼλ��⿪ʼ---------------------------------------------------------
		while(RXD1);					//�ȴ���ʼλ
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//��һ�ζ�ʱʱ�䵽
		TR0=0;
		test=1;
		temp=(char)RXD1;
		test=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//��ʼλʱ�䵽
		TR0=0;						
		if(temp==1) return(-1);			//���Ǻϸ����ʼλ������-1
//��ʼλ������---------------------------------------------------------
//8λ���ݽ��տ�ʼ--------------------------------------------------------
		for(i=0;i<=7;i++)
		{
			TH0=thx;TL0=tlx;TF0=0;TR0=1;
			while(!TF0);					//��һ�ζ�ʱʱ�䵽
			TR0=0;
			test=1;
			temp=RXD1;
			test=0;
			x=x>>1;
			if(RXD1) x=x | 0x80;
			TH0=thx;TL0=tlx;TF0=0;TR0=1;
			while(!TF0);					//һ������λʱ�䵽
			TR0=0;		  
		}
//8λ���ݽ������--------------------------------------------------------
//ֹͣλ��⿪ʼ---------------------------------------------------------
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//��һ�ζ�ʱʱ�䵽
		TR0=0;
		test=1;
		temp=RXD1;						//��ֹͣλ�ķ�֮��ʱ�䴦������
		test=0;
		TH0=thx;TL0=tlx;TF0=0;TR0=1;
		while(!TF0);					//ֹͣλʱ�䵽
		TR0=0;			   				//
		if(temp!=1) return(-1);
//ֹͣλ������---------------------------------------------------------
		RIx=1;						   	//�ý��վ�����־
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
