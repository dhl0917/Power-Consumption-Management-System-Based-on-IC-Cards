#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <absacc.h>


#define f 		11.0592


sbit buz = P1^0;
sbit ic_card_in = P1^4;

void warning();

void main()
{
    
	while(1)
	{
	 	if(ic_card_in)
		{
		  
		    warning();
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
