
#include<reg52.h>
#include <string.h>     
#include "mfrc522.h"
#define unit unsigned int 
#define uchar unsigned char 
void xunji();	
void S_CARD();	
 sbit  L_IN1=P1^0;
 sbit  L_IN2=P1^1;
 sbit  L_IN3=P1^2;
 sbit  L_IN4=P1^3;
 sbit  R_IN1=P1^4;
 sbit  R_IN2=P1^5;
 sbit  R_IN3=P1^6;
 sbit  R_IN4=P1^7;

 sbit  switch1=P0^0;
 sbit  switch2=P0^1;
 sbit  switch3=P0^2;
 sbit  switch4=P0^3;
 //sbit  beep=P0^4;

 sbit  led1=P1^0;
 sbit  led2=P1^1;
 //sbit  led3=P0^7;



 sbit  Rkey=P3^2;	
 sbit  Lkey=P3^3;


 uchar zkb1=0;
 uchar zkb2=0;
 
 
 //定义音阶常量  
 /*低音
 #define Do 63625 
#define Re 63833    
#define Mi 64104  
#define Fa 64820  
#define Sol 64260  
#define La 64400  
#define Si 64524
*/

#define Do 64580 
#define Re 64685  
#define Mi 64778  
#define Fa 64820  
#define Sol 64898  
#define La 64968  
#define Si 65030  
 
 /*高音
#define Do  65058
#define Re 65110  
#define Mi 65157  
#define Fa 65178  
#define Sol 65217  
#define La 64968  
#define Si 65283
*/
int scale[]={Do,Re,Mi,Fa,Sol,La,Si};        //定义音阶数组  
char i=0;


unsigned char g_ucTempbuf[10];
uchar  mmk[7][4]={{0x63,0x61,0x35,0x8d},{0x33,0xd0,0x99,0x8d},{0x93,0xc2,0x90,0x8d},{0x43,0x19,0x96,0x8d},{0xb3,0xe2,0x9a,0x8d},{0x47,0x7f,0x88,0xab},{0xfb,0xd0,0x19,0x22}};
uchar t;		//定时器的变量


void Delay (unit x)	 //延时函数
 {
   uchar i;
   	 while(x--)
   	{
   	 for (i=0;i<70;i++);    
   }  
}

void speed_init()
{
	if(switch1==0)  zkb1=10;
	if(switch2==0) 	zkb1=15;
	if(switch3==0) 	zkb1=25;
	if(switch4==0) 	zkb1=30;
}
 void zhengzhuan()	//正转
 {

	L_IN2= L_IN4=0;
	L_IN1= L_IN3=1;	

	R_IN2= R_IN4=0;
	R_IN1= R_IN3=1;
	 
 }
 void zuozhuan()	//左转
 {

	L_IN2= L_IN4=0;
	L_IN1= L_IN3=0;	

	R_IN2= R_IN4=0;
	R_IN1= R_IN3=1;
	 
 }
  void youzhuan()	//右转
 {

	L_IN2= L_IN4=0;
	L_IN1= L_IN3=1;	

	R_IN2= R_IN4=0;
	R_IN1= R_IN3=0;
	 
 }
 void stop()
 {
	L_IN2= L_IN4=1;
	L_IN1= L_IN3=1;	

	R_IN2= R_IN4=1 ;
	R_IN1= R_IN3=1;
 }

 void  init()
{
	
	PcdReset();		//复位RC522
	PcdAntennaOff(); 	//关闭天线
	PcdAntennaOn();	//开启天线
	IT1=0;//外部中断1采用电平触发方式；

	IT0=0;
	EX1=1;//开外部中断1
	EX0=1;//开外部中断0

	TMOD=0x11;	// 使用定时器0和定时器1
	TH0=(65536-100)/256; //赋初值1ms
	TL0=(65536-100)%256;
	TH1=(65536-50000)/256; //赋初值50ms
	TL1=(65536-50000)%256;  

	ET0=1;   //开启定时器0中断
	TR0=1;  
	ET1=1;     
	TR1=0;	  //关闭定时器1
	EA=1;					  //开总中断
 }
/***********************主函数***********************/

void main()
{	   


	P0=0X00;

	init();
	speed_init();
   while(1)
   {   	 
	S_CARD();
	      
	}   
}

 void int0 (void) interrupt 0 //外部中断0服务函数
{
	
	zuozhuan();	
}
 void int1 (void) interrupt 2 //外部中断1服务函数
{
	youzhuan();	
}

void S_CARD()
{	   unsigned char status,i=0;
	   unsigned char m,x,temp;
	status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡

	if (status == MI_OK)
	{  //  TR1=1;	  //开启定时器1  
		led1=1;
		status = PcdAnticoll(g_ucTempbuf);//防冲撞
		if (status == MI_OK)
		{    
		for(x=0;x<7;x++)//对卡号进行验证，是否为已注册的卡
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
			  if(temp>=4)  //卡号认证成功
			  {
				   i=x;
				  switch (i)
				  {
					  case 0:led1=1;	break;
					  case 1:led2=1;	break;
				//	  case 2:led3=1;	break;
					  case 3:led1=1;led2=1;	break;
				//	  case 4:led2=2;led3=1;	break;
				//	  case 5:led1=1;led3=1;	break;
				//	  case 6:led1=1;led2=1;led3=1;	break;
					  default :break;
				  }
				
				   break;
			  }					      
		}
		TR1=1;	  //开启定时器1 
		ET1=1;		
		}
		Delay(100);
		status = PcdSelect(g_ucTempbuf);//选定卡片
	
		PcdHalt();//卡休眠  
	
		TR1=0;	  //关闭定时器1
		ET1=0;
		P0=0X00;
	
}			
}
void T0_time(void) interrupt 1 //定时器0中断服务函数
{
	
	TH0=(65535-99)/256; //赋初值
	TL0=(65535-99)%256; 
	t++;
	if(t<zkb1)
	{ 
		zhengzhuan();
	}
	else
	{
		stop();
	}	 
	if(t>99)
	t=0;	
}	


void T1_time() interrupt 3
{  
  TH1= scale[i]/256; //赋初值50ms
  TL1= scale[i]%256; 
//	beep=~beep;	 
}