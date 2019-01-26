  #include<reg51.h>
  #include<stdio.h>
  #include<intrins.h>
  #include<absacc.h>

  typedef unsigned char uchar;
  extern serial_initial();
  sbit TMDAT=P3^7;

  void dmsec(unsigned int count);
  void tmreset(void);
  void tmstart(void);
  unsigned char tmrtemp(void);

  #define SEGMENT XBYTE[0xBFFF]
  #define BIT_LED XBYTE[0X7FFF]
  unsigned char code seg_code[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

  void main(void)
  {
  unsigned char last;
  int last_shi,last_ge;
  serial_initial();
  while(1){
 
  tmstart();
  dmsec(10);
  last=tmrtemp();
  printf("the current temperature:%bd\n",last);
  last_shi=last/10;
  last_ge=last%10;
  BIT_LED=0x02;
  SEGMENT=seg_code[last_shi];
  dmsec(1);
  BIT_LED=0x01;
  SEGMENT=seg_code[last_ge];
  dmsec(1);
  }
  }
	
  void dmsec(unsigned int count)
  {
		unsigned int i;
		while(count--)
		{
			for(i=0;i<125;i++){}
		}
  }
	
  void tmrest(void)
  {
		unsigned int i;
		TMDAT=0;
		i=103;
		while(i>0)i--;
		TMDAT=1;
		i=4;
		while(i>0)i--;
  }

  void tmpre(void)
  {
		unsigned int i;
		while(TMDAT);
		while(~TMDAT);
		i=4;
		while(i>0)i--;
  }

  bit tmrbit(void)
  {
  unsigned int i;
  bit dat;
  TMDAT=0;i++;
  TMDAT=1;i++;i++;
  dat=TMDAT;
  i=8;
  while(i>0)i--;
  return(dat);
  }

  unsigned char tmrbyte(void)
  {
  unsigned char i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
  j=tmrbit();
  dat=(j<<7)|(dat>>1);
  }
  return (dat);
  }
  void tmwbyte(unsigned char dat)
  {
  unsigned int i;
  unsigned char j;
  bit testb;
  for(j=1;j<=8;j++)
  {
  testb=dat&0x01;
  dat=dat>>1;
  if(testb)
  { 
  TMDAT=0;
  i++;i++;
  TMDAT=1;
  i=8;
  while(i>0)i--;
  }
  else
  {
  TMDAT=0;
  i=8;
  while(i>0)i--;
  TMDAT=1;
  i++;i++;
  }
  }
  }





  void tmstart(void)
  {
  tmrest();
  tmpre();
  dmsec(1);
  tmwbyte(0xcc);
  tmwbyte(0x44);
  }

  unsigned char tmrtemp()
  {
  unsigned char a,b,y1,y2,y3;
  tmrest();
  tmpre();
  dmsec(1);
  tmwbyte(0xcc);
  tmwbyte(0xbe);
  a=tmrbyte();
  b=tmrbyte();
  y1=a>>4;
  y2=b<<4;
  y3=y1|y2;
  return(y3);
  }