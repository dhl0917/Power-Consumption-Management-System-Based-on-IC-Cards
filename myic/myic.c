#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>

//��ʼ��������
#define ulong unsigned long 
#define uint unsigned int 
#define uchar unsigned char 
#define SEGMENT XBYTE[0xbfff] //�����ַ
#define BIT_LED XBYTE[0x7fff] //λ���ַ
bdata char com_data;
sbit mos_bit=com_data^7;
sbit low_bit=com_data^0;

sbit SCL_IC_CARD=P1^5; //24C01����3��
sbit SDA_IC_CARD=P1^6; //24C01����6��
sbit WP_IC_CARD =P1^7; //24C01����2��
sbit CHECK_IC_CARD =P1^4; //24C01����1��

sbit D2=P1^1; //����ָʾ��
sbit WARNING=P1^0; //�������������ź�

sbit sw1=P3^3; //sw1
sbit sw2=P3^5; //sw2
sbit sw3=P3^4; //sw3

unsigned char data display_buffer[4];
unsigned char data card_ID[20];
unsigned char data order[20];
unsigned char data send[20];
unsigned char code tab1[20]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};//�����������ʾ���ݱ� 
unsigned char i;
unsigned char j;
unsigned char data temp[2];
int p;
int x=0;
//unsigned Flag1=1;
unsigned flag=1;
unsigned cflag=0;

void delayms(unsigned int i); //�����ӳٺ���
//void delay1(uint z); //��ʱ����1
void delay(int n); //��ʱ����2
void display(); //�����ɨ����ʾ����

//void beep(); //��ӭ��������
//void success(); //���ݴ���ɹ���ʾ��
//void warning(); //������������
void wr_24c01(char a,char b); //����д����
unsigned char rd_24c01(char a);//���������


/*****************************����������*******************************/
void main()
{
	  EA=1; //�����ж�Դ����
		EX0=0; //��ֹ�ⲿ�ж�
		PS=1; //�����п��ж���Ϊ�����ȼ�
	
		D2=0;//D2��
		WARNING=1;//�ر���
		BIT_LED=0;	//�������
//		WARNING=0; //����ɹ����뵥Ƭ����ʾ��
//		WARNING=1;

	while(1)
	{
	
		while(CHECK_IC_CARD==0)		//δ�忨ʱ��˸��0000��
		{
			BIT_LED=0xff;;	
			SEGMENT=0x3F;
			delayms(500);
			BIT_LED=0x00;
			delayms(500);
			flag=1;
		}
		BIT_LED=0;	//�������
	
	
	
	while(CHECK_IC_CARD!=0)	//�忨ִ��
		{
			
/*			for (i=0;i<10;i++) //��ԭʼ����,10��Ԫ��
					{
						card_ID[i]=rd_24c01(i);
						D2=~D2;	//D2��
					}
					D2=0;	//D2��
*/
					
//				WP_IC_CARD=1;
//					wr_24c01(0,0x00);
					delay(250);
//					wr_24c01(1,0x00);
					delay(250);
//					wr_24c01(2,0x22);
					delay(250);
//					wr_24c01(3,0x33);
					delay(250);
//					wr_24c01(4,0x44);
					delay(250);
//					wr_24c01(5,0x55);
					delay(250);
//					wr_24c01(6,0x33);
					delay(250);
//					wr_24c01(7,0x44);	
					delay(3000);
					
					
		//			wr_24c01(8,0x55);
			//		delay(250);
			//		wr_24c01(9,0x66);
			//		delay(1500);
	
					
	/*		if((card_ID[0]==0x00)&&(card_ID[1]==0x31)&&(card_ID[2]==0x31)&&(card_ID[3]==0x31))	//�ж�У��λ��1 2 3��Ԫ�أ��Ƿ�Ϸ����жϹ�ʧλ��0��Ԫ�أ��Ƿ��ʧ
			{
					
					BIT_LED=0; //�ر�LED��
					for(j=300;j>0;j--) //��ʾAAAA
								{
										display_buffer[0]=10; 
										display_buffer[1]=10;
										display_buffer[2]=10;
										display_buffer[3]=10;
										display();
								}
					BIT_LED=0; //�ر�LED��
		*/

		//���Ϳ��ŵ�����				
				if(flag==1)
					
				{
					
					for(i=0;i<6;i++) //������
							{
									send[i]=rd_24c01(i);
							}
					send[6]=0x33;		//��У����
					send[7]=0x44;
					
					
					ES=0; //��ֹ���п��ж�
					SCON=0x50; //���óɴ��ڹ�����ʽ
					TMOD=0x20; //����T1Ϊ����װ8λ��ʱ��
					TL1=0xFD; //������=9600
					TH1=0xFD;
					TR1=1; //������ʱ��
					TI=0;
					
				
					
					
						for(i=0;i<8;i++) //���з���
							{
								//	SBUF=rd_24c01(i);
								SBUF=send[i];
									while(!TI);
									TI=0;
									D2=~D2;//D2��˸
							}
							
							D2=1;//D2��
				}
								
		//�Ӵ��ڽ�����Ϣ

						BIT_LED=0;
				
						SCON=0x50;                                                //���óɴ��ڹ�����ʽ
            TMOD=0x20;                                                //����T1Ϊ����װ8λ��ʱ��
            TL1=0xFD;                                                 //������=9600
	          TH1=0xFD; 
            TR1=1;   	                                               //������ʱ��	                                                      	
	          EA=1;                                                     //�����ж�
	          EX0=0;                                                    //��ֹ�ⲿ�ж�  
						ES=1;	
				
				if((order[6]==0x33)&&(order[7]==0x44))	//��֤У��λ
				{
					
					if(order[0]==0x00)		//�޶���
						{	
							;
						}
	 				if(order[0]==0xff)		//ֹͣ����
						{
							flag=0;
							order[0]=0x00;
			//				order[6]==0x00;
			//				order[7]==0x00;
						}
						if(order[0]==0xee)	//��������
						{
							flag=1;
							order[0]=0x00;
			
						}
						if(order[0]==0x11)	//���ⱨ��
						{
							flag=0;
							
							for(i=0;i<10;i++)
							{
								D2=~D2;
								WARNING=~WARNING;
								delayms(100);
							}
							
							D2=1;	//D2��
							WARNING=1;//WARNING��
							order[0]=0x00;
							order[6]=0x00;
							order[7]=0x00;
						}
						
						if(order[0]==0xaa)	//��cflag��ʾ���
						{
							cflag=1;
						order[0]=0x00;
							
						}
						
		/*				if(order[0]==0xbb)
						{
							cflag=0;
							order[0]=0xee;
							
						}
			*/
						if(order[0]==0x33)	//д��
						{	
						WP_IC_CARD=1;
						wr_24c01(0,0x00);
						delay(250);
						wr_24c01(1,0x00);
						delay(250);
						wr_24c01(2,order[1]);
						delay(250);
						wr_24c01(3,order[2]);
						delay(250);
						wr_24c01(4,order[3]);
						delay(250);
						wr_24c01(5,order[4]);
						delay(250);
						wr_24c01(6,0x33);
						delay(250);
						wr_24c01(7,0x44);	
						delay(3000);
						order[0]=0x00;
						}
						
					}
				else
				{
					;
				}
				
				while(cflag==1)						//��ʾ���
				{
					
	/*					BIT_LED=0;
						SCON=0x50;                                                //���óɴ��ڹ�����ʽ��8λURT
            TMOD=0x20;                                                //����T1Ϊ����װ8λ��ʱ��
            TL1=0xFD;                                                 //256-FOSC/BTL/32/12  9600bps(11.0592MHz)
	          TH1=0xFD; 
            TR1=1;   	                                               //������ʱ��	                                                      	
	          EA=1;                                                     //�����ж�
	          EX0=0;                                                    //��ֹ�ⲿ�ж�  
						ES=1;	
*/										
					if(sw1)
					{
						for(j=300;j>0;j--) //��ʾ���
								{
										display_buffer[0]=order[1]-0x30; 
										display_buffer[1]=order[2]-0x30+10;		//С����
										display_buffer[2]=order[3]-0x30;
										display_buffer[3]=order[4]-0x30;
										display();
								}
					}
					else									//����ֹͣ��ʾ
					{
						cflag=0;
						order[0]=0xee;
		//				order[0]=0xee;
		//				order[6]=0x33;
			//				order[7]=0x44;
					}
					
				}
			BIT_LED=0; //�ر�LED��
				if(!sw2)							//���»ָ���ʾ
				{
					cflag=1;
		//			order[0]=0xbb;
		//				order[6]=0x33;
		//					order[7]=0x44;
				}
				else
				{
					;
				}
				if(!sw3)								//������ʾ���ź���λ
				{
						if(rd_24c01(4)>0x09)
						{
							temp[0]=rd_24c01(4)-0x10;
							temp[1]=0x01;
						}
						else{
							temp[0]=rd_24c01(4);
							temp[1]=0x00;
						
						}
					
					for(j=300;j>0;j--) //��ʾ���ź���λ
								{
										display_buffer[0]= rd_24c01(5)-0x00;
										display_buffer[1]= 0;
										display_buffer[2]= temp[0];
										display_buffer[3]= temp[1];
										display();
								}
				
				}
				else{
					;
				}


/*						if(order[0]==0xff){p=1;}	
						else{p=0;}
//						p=1;
					if(p==1)
					{

						for(i=0;i<6;i++) //��IC��������������
						{
				
									TI=0;
								D2=~D2;
								delayms(300);
								WARNING=~WARNING;
								delayms(300);
							}
					}
							
*/							
			
			}
/*			else	//���Ϸ������ⱨ������ʾFFFF
			{
				for(i=10;i>0;i--)
						{
								D2=~D2; //����ָʾ����
							//	WARNING=~WARNING;	//����
								delayms(100);

						}
						D2=1;	//��D2
						WARNING=1;	//�ر���
						BIT_LED=0;
						for(j=300;j>0;j--) //��ʾ�����ʾFFFF
							{
								display_buffer[0]=15; display_buffer[1]=15; display_buffer[2]=15; display_buffer[3]=15; 
								display();
							}
						BIT_LED=0;
				}
			}
*/
		}
		
}










/*****************************�Ӻ�������****************** *************/
void start() //������дʱ��
{
SDA_IC_CARD=1;
SCL_IC_CARD=1;
SDA_IC_CARD=0;
SCL_IC_CARD=0;
}
void stop() //ֹͣ����
{
SDA_IC_CARD=0;
SCL_IC_CARD=1;
SDA_IC_CARD=1;
}
void ack() //Ӧ����
{
SCL_IC_CARD=1;
SCL_IC_CARD=0;
}
void shift8(char a) //8λ��λ���
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
unsigned char rd_24c01(char a) //��IC������
{
data unsigned char i,command;
SDA_IC_CARD=1;
SCL_IC_CARD=0;
start();
command=160;
shift8(command);
ack();
shift8(a);
ack();
start();
command=161;
shift8(command);
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
void wr_24c01(char a,char b) //дIC������
{
	data unsigned char command;
	WP_IC_CARD=0;
	_nop_();
	SDA_IC_CARD=1;
	SCL_IC_CARD=0;
	start();
	command=160;
	shift8(command);
	ack();
	shift8(a);
	ack();
	shift8(b);
	ack();
	stop();
	_nop_();
	WP_IC_CARD=1;
}
void delay(int n) //��ʱ����
{
int i;
for (i=1;i<=n;i++){;}
}
void int_com(void) interrupt 4
{

	if(RI)
	{
		
		order[x]=SBUF;
		x++;
		if(x>7)
		{
			x=0;
		}
		RI=0;
	
	}

}
void delayms(unsigned int i) //�����ӳٺ���
{
unsigned int n;
while(i--){
for(n=0;n<125;n++);
}
}
void display() //�����ѭ����ʾ����
{
unsigned char e=0x01;
unsigned int i;
for(i=0;i<=3;i++)
{
BIT_LED=e;
SEGMENT=tab1[display_buffer[i]];
delayms(3);
e<<=1;
}
}
/*
void beep(void)
{
unsigned int i=0;

ulong ul;
uint n;


}*/

/*void success() //���ݴ���ɹ���ʾ��
{
uchar a,b,c; //����ѭ���ñ���                                      
i=1;
while(i>0)//whileѭ��
{
i=i-1;
for(a=200;a>0;a--)//ѭ������
{
WARNING=0;//��������
for(c=500;c>0;c--);//��ʱ
WARNING=1;//����������
for(c=500;c>0;c--);//��ʱ
}
for(b=200;b>0;b--)//ѭ������
{
WARNING=0;//��������
delay1(1.5);//����1.5ms��ʱ�ӳ���
WARNING=1;//����������
delay1(1.5);//����1.5ms��ʱ�ӳ���
}
delay1(2000);//����2s��ʱ�ӳ���
}
}*/
/*void delay1(uint z) //1ms��ʱ�ӳ���ͨ��zֵ�ı���ʱʱ��
{
uint x,y;
for(x=z;x>0;x--)
for(y=110;y>0;y--);
}*/
/*
void warning(void) //������ʾ��
{
WARNING=0;
delayms(3000);
WARNING=1;
}*/

