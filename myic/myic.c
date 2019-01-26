#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>

//初始变量定义
#define ulong unsigned long 
#define uint unsigned int 
#define uchar unsigned char 
#define SEGMENT XBYTE[0xbfff] //段码地址
#define BIT_LED XBYTE[0x7fff] //位码地址
bdata char com_data;
sbit mos_bit=com_data^7;
sbit low_bit=com_data^0;

sbit SCL_IC_CARD=P1^5; //24C01卡的3脚
sbit SDA_IC_CARD=P1^6; //24C01卡的6脚
sbit WP_IC_CARD =P1^7; //24C01卡的2脚
sbit CHECK_IC_CARD =P1^4; //24C01卡的1脚

sbit D2=P1^1; //工作指示灯
sbit WARNING=P1^0; //警告声音发生信号

sbit sw1=P3^3; //sw1
sbit sw2=P3^5; //sw2
sbit sw3=P3^4; //sw3

unsigned char data display_buffer[4];
unsigned char data card_ID[20];
unsigned char data order[20];
unsigned char data send[20];
unsigned char code tab1[20]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};//定义数码管显示数据表 
unsigned char i;
unsigned char j;
unsigned char data temp[2];
int p;
int x=0;
//unsigned Flag1=1;
unsigned flag=1;
unsigned cflag=0;

void delayms(unsigned int i); //毫秒延迟函数
//void delay1(uint z); //延时函数1
void delay(int n); //延时函数2
void display(); //数码管扫描显示函数

//void beep(); //欢迎铃声函数
//void success(); //数据传输成功提示音
//void warning(); //报警函数声明
void wr_24c01(char a,char b); //定义写函数
unsigned char rd_24c01(char a);//定义读函数


/*****************************主函数部分*******************************/
void main()
{
	  EA=1; //允许中断源工作
		EX0=0; //禁止外部中断
		PS=1; //将串行口中断设为高优先级
	
		D2=0;//D2亮
		WARNING=1;//关报警
		BIT_LED=0;	//关数码管
//		WARNING=0; //程序成功载入单片机提示音
//		WARNING=1;

	while(1)
	{
	
		while(CHECK_IC_CARD==0)		//未插卡时闪烁“0000”
		{
			BIT_LED=0xff;;	
			SEGMENT=0x3F;
			delayms(500);
			BIT_LED=0x00;
			delayms(500);
			flag=1;
		}
		BIT_LED=0;	//关数码管
	
	
	
	while(CHECK_IC_CARD!=0)	//插卡执行
		{
			
/*			for (i=0;i<10;i++) //读原始数据,10个元素
					{
						card_ID[i]=rd_24c01(i);
						D2=~D2;	//D2闪
					}
					D2=0;	//D2亮
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
	
					
	/*		if((card_ID[0]==0x00)&&(card_ID[1]==0x31)&&(card_ID[2]==0x31)&&(card_ID[3]==0x31))	//判断校验位（1 2 3号元素）是否合法，判断挂失位（0号元素）是否挂失
			{
					
					BIT_LED=0; //关闭LED灯
					for(j=300;j>0;j--) //显示AAAA
								{
										display_buffer[0]=10; 
										display_buffer[1]=10;
										display_buffer[2]=10;
										display_buffer[3]=10;
										display();
								}
					BIT_LED=0; //关闭LED灯
		*/

		//发送卡号到串口				
				if(flag==1)
					
				{
					
					for(i=0;i<6;i++) //读卡号
							{
									send[i]=rd_24c01(i);
							}
					send[6]=0x33;		//加校验码
					send[7]=0x44;
					
					
					ES=0; //禁止串行口中断
					SCON=0x50; //设置成串口工作方式
					TMOD=0x20; //设置T1为可重装8位定时器
					TL1=0xFD; //波特率=9600
					TH1=0xFD;
					TR1=1; //开启定时器
					TI=0;
					
				
					
					
						for(i=0;i<8;i++) //序列发送
							{
								//	SBUF=rd_24c01(i);
								SBUF=send[i];
									while(!TI);
									TI=0;
									D2=~D2;//D2闪烁
							}
							
							D2=1;//D2灭
				}
								
		//从串口接受信息

						BIT_LED=0;
				
						SCON=0x50;                                                //设置成串口工作方式
            TMOD=0x20;                                                //设置T1为可重装8位定时器
            TL1=0xFD;                                                 //波特率=9600
	          TH1=0xFD; 
            TR1=1;   	                                               //开启定时器	                                                      	
	          EA=1;                                                     //开总中断
	          EX0=0;                                                    //禁止外部中断  
						ES=1;	
				
				if((order[6]==0x33)&&(order[7]==0x44))	//验证校验位
				{
					
					if(order[0]==0x00)		//无动作
						{	
							;
						}
	 				if(order[0]==0xff)		//停止发送
						{
							flag=0;
							order[0]=0x00;
			//				order[6]==0x00;
			//				order[7]==0x00;
						}
						if(order[0]==0xee)	//继续发送
						{
							flag=1;
							order[0]=0x00;
			
						}
						if(order[0]==0x11)	//声光报警
						{
							flag=0;
							
							for(i=0;i<10;i++)
							{
								D2=~D2;
								WARNING=~WARNING;
								delayms(100);
							}
							
							D2=1;	//D2灭
							WARNING=1;//WARNING关
							order[0]=0x00;
							order[6]=0x00;
							order[7]=0x00;
						}
						
						if(order[0]==0xaa)	//立cflag显示余额
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
						if(order[0]==0x33)	//写卡
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
				
				while(cflag==1)						//显示余额
				{
					
	/*					BIT_LED=0;
						SCON=0x50;                                                //设置成串口工作方式在8位URT
            TMOD=0x20;                                                //设置T1为可重装8位定时器
            TL1=0xFD;                                                 //256-FOSC/BTL/32/12  9600bps(11.0592MHz)
	          TH1=0xFD; 
            TR1=1;   	                                               //开启定时器	                                                      	
	          EA=1;                                                     //开总中断
	          EX0=0;                                                    //禁止外部中断  
						ES=1;	
*/										
					if(sw1)
					{
						for(j=300;j>0;j--) //显示余额
								{
										display_buffer[0]=order[1]-0x30; 
										display_buffer[1]=order[2]-0x30+10;		//小数点
										display_buffer[2]=order[3]-0x30;
										display_buffer[3]=order[4]-0x30;
										display();
								}
					}
					else									//按下停止显示
					{
						cflag=0;
						order[0]=0xee;
		//				order[0]=0xee;
		//				order[6]=0x33;
			//				order[7]=0x44;
					}
					
				}
			BIT_LED=0; //关闭LED灯
				if(!sw2)							//按下恢复显示
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
				if(!sw3)								//按下显示卡号后四位
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
					
					for(j=300;j>0;j--) //显示卡号后四位
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

						for(i=0;i<6;i++) //将IC卡内容送至串口
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
/*			else	//不合法，声光报警，显示FFFF
			{
				for(i=10;i>0;i--)
						{
								D2=~D2; //错误指示灯亮
							//	WARNING=~WARNING;	//报警
								delayms(100);

						}
						D2=1;	//关D2
						WARNING=1;	//关报警
						BIT_LED=0;
						for(j=300;j>0;j--) //显示错误标示FFFF
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










/*****************************子函数部分****************** *************/
void start() //启动读写时序
{
SDA_IC_CARD=1;
SCL_IC_CARD=1;
SDA_IC_CARD=0;
SCL_IC_CARD=0;
}
void stop() //停止操作
{
SDA_IC_CARD=0;
SCL_IC_CARD=1;
SDA_IC_CARD=1;
}
void ack() //应答函数
{
SCL_IC_CARD=1;
SCL_IC_CARD=0;
}
void shift8(char a) //8位移位输出
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
unsigned char rd_24c01(char a) //读IC卡函数
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
void wr_24c01(char a,char b) //写IC卡函数
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
void delay(int n) //延时函数
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
void delayms(unsigned int i) //毫秒延迟函数
{
unsigned int n;
while(i--){
for(n=0;n<125;n++);
}
}
void display() //数码管循环显示函数
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

/*void success() //数据传输成功提示音
{
uchar a,b,c; //定义循环用变量                                      
i=1;
while(i>0)//while循环
{
i=i-1;
for(a=200;a>0;a--)//循环次数
{
WARNING=0;//蜂鸣器响
for(c=500;c>0;c--);//延时
WARNING=1;//蜂鸣器不响
for(c=500;c>0;c--);//延时
}
for(b=200;b>0;b--)//循环次数
{
WARNING=0;//蜂鸣器响
delay1(1.5);//调用1.5ms延时子程序
WARNING=1;//蜂鸣器不响
delay1(1.5);//调用1.5ms延时子程序
}
delay1(2000);//调用2s延时子程序
}
}*/
/*void delay1(uint z) //1ms延时子程序，通过z值改变延时时间
{
uint x,y;
for(x=z;x>0;x--)
for(y=110;y>0;y--);
}*/
/*
void warning(void) //报警提示音
{
WARNING=0;
delayms(3000);
WARNING=1;
}*/

