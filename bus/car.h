#ifndef __CAR_H__  
#define __CAR_H__  
#include<reg52.h>
#include<stdio.h>
#include<intrins.h> 
#define uchar unsigned char 
#define unit unsigned int 
#define TURE 1
#define FALSE 0

sbit   IR=P3^2;//红外接口标志
 sbit  IN1=P1^0;
 sbit  IN2=P1^1;
 sbit  IN3=P1^2;
 sbit  IN4=P1^3;
 sbit  ENA=P1^4;	//右轮
 sbit  ENB=P1^5;	//左轮
 sbit  Rkey=P1^6;
 sbit  Lkey=P1^7;

 uchar zkb1=0;
 uchar zkb2=0;

uchar t,shu,s;		//定时器的变量
uchar flag;


unsigned int hour,minute,second,count;
//

char data TimeNum[]="                ";
char data Test1[]="                  ";


uchar code DUAN_XUAN[]= {0xc0,0xf9,0xa4,0xb0,
	0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e
};//共阳极数码管编码数组  
uchar WEIXUAN[]={0X01,0X02,0X04,0X08};	//从左到右
uchar u[4]={1,2,3,4};


unsigned char  irtime;//红外用全局变量



void TIM0init(void);
void EX0init(void);
void Delay(unsigned char mS);
void display(uchar y);

void Ir_work(void);
void Ircordpro(void);
void DelayUs(unsigned char us);
void DelayMs(unsigned char ms);

 void  init(void);
  void zhengzhuan(void);
void xunji(void);  	  
 #endif