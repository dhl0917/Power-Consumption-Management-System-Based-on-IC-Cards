#include <at89x52.h>
#include <stdio.h>
#include <absacc.h>
#include <intrins.h>
#define fosc 		11.0592				//��λΪMHz,ʱ��Ƶ�ʾ����󣬶�ʱ�����ʱ��Ϊ
										//65536*(12/fosc),ԼΪ71mS.TN<=65536*12/fosc
										//(TNС�ڵ���60397����EBEDH).��ʱ��1��ʱʱ��
										//(Tx<=65536*12/fosc=60397us)
//extern void serial_initial(void);

extern void UART_initial(unsigned int x);
extern signed int receiv();
extern void restore();
extern void send(unsigned char x);

signed int read_buf;
unsigned char x,i; 
signed int read_buf;
bit RIx,TIx,error; 

sbit test=P2^0;
void delay(unsigned int n);
main()
{
	//serial_initial();
	UART_initial(1200);
	while(1)
		{
			read_buf=receiv();
			RIx=0;
			x=read_buf;
			delay(200);
			send(i);
			TIx=0;
			if((i%16)==0) printf("\n");
			printf("  %+BX",x);
			i++; 
			_nop_();	
		}
//	restore();	
}
void delay(unsigned int n)
{	unsigned int i;
	for(i=1;i<=n;i++){;}
}