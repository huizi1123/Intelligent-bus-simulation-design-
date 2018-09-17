#include<reg52.h>
#include <string.h>     
#include "mfrc522.h"


#define unint unsigned int
#define unchar unsigned char
#define uchar unsigned char
#define uint unsigned int

sbit ENA = P2^7;
sbit ENB = P2^6;				    
sbit IN1 = P2^2;
sbit IN2 = P2^3;
sbit IN3 = P2^4;
sbit IN4 = P2^5;
sbit jiance1 = P2^0;  //ѭ��ģ��
sbit jiance2 = P2^1;  //ѭ��ģ��

sbit K1 = P0^0;
sbit K2 = P0^1;
sbit K3 = P0^2;

sbit led1 = P1^0;
sbit led2 = P1^1;
sbit led3 = P1^7;
int time1 = 0,PWM1  ,PWM2 ;
void zhengzhuan();
void youzhuan();
void zuozhuan();
void xunji();
void TimeConfig();
void S_CARD();


unsigned char g_ucTempbuf[10];
uchar  mmk[7][4]={{0x63,0x61,0x35,0x8d},
				{0x33,0xd0,0x99,0x8d},		
				{0x93,0xc2,0x90,0x8d},
				{0x43,0x19,0x96,0x8d},{0xb3,0xe2,0x9a,0x8d},{0x47,0x7f,0x88,0xab},{0xfb,0xd0,0x19,0x22}};


void Delay (uint x)	 //��ʱ����
 {
   uchar i;
   	 while(x--)
   	{
   	 for (i=0;i<70;i++);    
   }  
}

 void speedint()
 {
 	if( K1 == 0 )
	  { PWM1 = 15;
	    PWM2 = 15;
	  }
	 	if( K2 == 0 )
	  { PWM1 = 25;
	    PWM2 = 25;
	  }
	 	if( K3 == 0 )
	  { PWM1 = 30;
	    PWM2 = 30;
	  }
 }
void main()
{	 
	TimeConfig();
	speedint();
    led1 = 1;
    led2 = 1;
	led3 = 1;
	ENA = 1;
	ENB = 1;
	while(1)
	{ 
	xunji();
	S_CARD();
	} 
}
void S_CARD()
{	unsigned char status,i=0;
	   unsigned char m,x,temp;
	status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��

	if (status == MI_OK)
	{ 
		status = PcdAnticoll(g_ucTempbuf);//����ײ
		if (status == MI_OK)
		{    
		for(x=0;x<7;x++)//�Կ��Ž�����֤���Ƿ�Ϊ��ע��Ŀ�
		{
			  temp=0;
			  for(m=0;m<4;m++)   
			  {
				  if(mmk[x][m] ==g_ucTempbuf[m] )
				  {
					   temp++;
				  }
				  if(g_ucTempbuf[m] !=mmk[x][m] )
				  {				 			
						  break;			
				  }
			  }
			  if(temp>=4)  //������֤�ɹ�
			  {
				   i=x;
				  switch (i)
				  {
					  case 0:led1=1;led2=0; led3 = 0;break;
					  case 1:led1=0;led2=1; led3 = 0;break;
					  case 2:led1=0;led2=0; led3 = 1;break;

					  case 3:led1=1;led2=1; led3 = 0;break;
					  case 4:led1=0;led2=1; led3 = 1;break;
					  case 5:led1=1;led2=0; led3 = 1;break;

					  case 6:led1=1;led2=1; led3 = 1;break;
					 // case 3:led1=1;led2=1;	break;
					  //case 4:led2=2;led3=1;	break;
					 // case 5:led1=1;led3=1;	break;
					  //case 6:led1=1;led2=1;led3=1;	break;
					  default :break;
				  }
				
				   break;
			  }					      
		}
   		
		}
		Delay(10);
		status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
	
		PcdHalt();//������  
//		P0=0X00;
	
}			
}
void  TimeConfig()
{
    PcdReset();		//?��??RC522
	PcdAntennaOff(); 	//�ر�����
	PcdAntennaOn();	//��������


	TMOD = 0x01;		//��ʱ��0������ʽ1
	TH0 = 0xff;
	TL0 = 0x38;			//װ�붨ʱ����ֵ��ʱ50ms

	ET0 = 1;			//��ʱ��0�ж�����
	TR0 = 1; 

	EA  = 1;				//���ж�����
}


void zhengzhuan()
{
  IN1=1;
  IN2=0;
  IN3=1;
  IN4=0;
}
void zuozhuan()
{
  IN1=1;
  IN2=0;
  IN3=0;
  IN4=1;
}
void youzhuan()
{
   IN1=0;
   IN2=1;
   IN3=1;
   IN4=0;
 }



void xunji()
{
		zhengzhuan();
		while(jiance1==0&&jiance2==1)
		{
			zuozhuan();	
		//	PWM1 = 35;
		//   	PWM2 = 25;
		}    
		while(jiance1==1&&jiance2==0)
		{
			youzhuan();
		//	PWM2 = 35;
		//   	PWM1 = 25;
		}
		while(jiance1==1&&jiance2==1)
		{
		 IN1=0;
         IN2=0;
         IN3=0;
         IN4=0;
		}
		//PWM1 = 30;
	//	PWM2 = 30;	    			
}



void time0Init(void) interrupt 1     //��ʱ��0�жϷ�����
{
	TH0 = 0xFF;
	TL0 = 0x38;
	time1++;
	if(!(time1 - 100) )
	time1 = 0;	
	if( time1 < PWM1 )
	{
	  	ENA =1;	
	}else{
	  	ENA =0;		
	}

	if( time1 < PWM2 )
	{
	   ENB = 1;	
	}else{
	    ENB = 0;			
	}				
}	

