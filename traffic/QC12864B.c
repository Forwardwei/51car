//12864串行时序的实现
///**************************************/
#include<reg51.h>
#include<intrins.h>
#include "QC12864B.h"
/**************************************/
#define  uchar    unsigned char
#define  uint     unsigned int 


//端口定义
sbit  LCD_CS=P2^6;
sbit  LCD_SID=P2^5;          //串行数据线
sbit  LCD_SCLK=P2^7;         //串上时钟输入
sbit  LCD_RST=P3^4; 
sbit  LCD_PSB=P3^2;
uchar code table1[]={"**双车模拟**"};      //定义要显示的字符串 
uchar code table2[]={"速度--路程--时间"}; 
/**************************************/
//延时函数
/**************************************/
void Delay_ms(uchar n)
{
   uchar i;
   uchar j;
   for(i=0;i<n;i++)
   for(j=0;j<125;j++)        //大概1ms
 _nop_();
}
/**************************************/
//串行发送一个字节
/**************************************/
void LCD_sendbyte(uchar byte)
{
   uchar i;
   for(i=0;i<8;i++)
    {
   LCD_SCLK=0;               //拉低时钟线
   _nop_();
   LCD_SID=(bit)(byte&0x80);    //发送最高位数据
   LCD_SCLK=1;               //上升沿发送数据
   byte=byte<<1;                  //左移一位
 }
}

/****************************************/
//写指令
/****************************************/
void LCD_write_com(uchar com)
{
  LCD_CS=1;        
   LCD_sendbyte(0xf8);       //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(0),0
   LCD_sendbyte(0xf0&com);       //取高四位，数据分两次传送，
                                 //每个字节的内容被送入两个字节
               //高四位放在第一个字节的高四位
   LCD_sendbyte(0xf0&(com<<4));        //低四位放在第二个字节的高四位 
    LCD_CS=0;       
  Delay_ms(10);             //串行不支持读操作，不可检测忙操作，这里用延时替代   
}
/******************************************/
//写数据
/******************************************/
void LCD_write_dat(uchar dat)
{
    LCD_CS=1;        
   LCD_sendbyte(0xfa);        //送入5个连续的“1“，启动一个周期,11111,RW(0),RS(1),0
   LCD_sendbyte(0xf0&dat);       //取高四位，数据分两次传送，
                                 //每个字节的内容被送入两个字节
               //高四位放在第一个字节的高四位
   LCD_sendbyte(0xf0&(dat<<4));        //低四位放在第二个字节
    LCD_CS=0;        
   Delay_ms(10);
}
/********************************************/
//LCD初始化
/********************************************/
void LCD_init(void)
{  
 Delay_ms(5);
 LCD_RST=1;
 LCD_RST=0;
 LCD_RST=1;
 LCD_PSB=0;	
 LCD_write_com(0x30);             //选择基本指令集   
 LCD_write_com(0x0c);       //开显示，无游标，不反白
 LCD_write_com(0x01);       //清除显示屏幕，把DDRAM位址计数器调整为00H
 Delay_ms(5);         //清屏操作时间较长1.6ms 因此加此延时
 LCD_write_com(0x02);       //清DDRAM位址归位,此处貌似与清屏重复
 LCD_write_com(0x06);       //设定光标右移，整体显示不移动
}

/***************************************************/
//设置显示地址
/***************************************************/
void LCD_Setaddress(uchar x,uchar y)
{              //地址从第1行第1列开始不从0开始
  uchar addr;
  switch(x)
  {
   case 1: addr=0x80+y-1;
           break;
   case 2: addr=0x90+y-1;
           break;
   case 3: addr=0x88+y-1;
           break;
   case 4: addr=0x98+y-1;
           break;
   default : break;
  }
   LCD_write_com(addr);    //字符显示开始地址
}
/*****************************************************/
//让字符串显示在固定位置
/*****************************************************/
void LCD_Putstring( uchar x, uchar y, uchar *pData )
{
 LCD_Setaddress(x,y);
 while( *pData != '\0' )
 {
   LCD_write_dat( *pData++ );
 }
}
void LCD_Putuint( uchar x, uchar y, uint z  )
{
	if(z<100)
	{
		uchar z1,z2;
		z1=z/10;
		z2=z%10;
		LCD_Setaddress(x,y);
		LCD_write_dat( 0x30);
	  LCD_write_dat( 0x30+z1 );
	  LCD_write_dat( 0x30+z2 );
	}
	else
	if(z>99&&z<999)
	{
		uchar z1,z2,z3;
		z1=z/100;
		z2=z%100/10;
		z3=z%100%10;
		LCD_Setaddress(x,y);
		LCD_write_dat( 0x30+z1 );
	  LCD_write_dat( 0x30+z2 );
		LCD_write_dat( 0x30+z3);
	}
	else
		LCD_Putstring(x,y,"超时");
 
 
}


void LCD_Putfloatv( uchar x, uchar y, float z  )
{
	uchar z1,z2;
	z1=(uchar)z;
	z2=(z-z1)*10;
	if(z>=10)
	{
		LCD_Putstring(x,y,"超速");
	}
	
	else
		{
			LCD_Setaddress(x,y);
      LCD_write_dat( 0x30+z1 );
			LCD_write_dat( 0x2e );
			LCD_write_dat( 0x30+z2 );
		}
			
	
}
void LCD_Putfloats( uchar x, uchar y, float z  )
{
	if(z>=20)
	{
		LCD_Putstring(x,y,"超程");
	}
	else 
	{
		
	  uchar z0,z1,z2,z3;
		z0=z*10;
		z1=z0/100;
		z2=(z0%100)/10;
		z3=(z0%100)%10;
	  LCD_Setaddress(x,y);
		LCD_write_dat(0x30+z1);
		LCD_write_dat(0x30+z2);
		LCD_write_dat(0x2e);
		LCD_write_dat(0x30+z3);
   }
			
	
}



void LCDinit(void)
{
	
	
	LCD_init();

	LCD_Putstring(1,2,table1);
	LCD_Putstring(2,1,table2);	
	LCD_Putstring(3,1,"A:");
	LCD_Putstring(4,1,"B:");
	LCD_Putstring(3,8," s");
	LCD_Putstring(4,8," s");

}
void receiveA(float Av,float As,uint At)
{
	
	LCD_Putfloatv(3,2,Av); 
	LCD_Putfloats(3,4,As);
  LCD_Putuint(3,7,At);
	
}
void receiveB(float Bv,float Bs,uint Bt)
{

	LCD_Putfloatv(4,2,Bv);
  LCD_Putfloats(4,4,Bs);
  LCD_Putuint(4,7,Bt);	
}
