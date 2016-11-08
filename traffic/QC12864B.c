//12864����ʱ���ʵ��
///**************************************/
#include<reg51.h>
#include<intrins.h>
#include "QC12864B.h"
/**************************************/
#define  uchar    unsigned char
#define  uint     unsigned int 


//�˿ڶ���
sbit  LCD_CS=P2^6;
sbit  LCD_SID=P2^5;          //����������
sbit  LCD_SCLK=P2^7;         //����ʱ������
sbit  LCD_RST=P3^4; 
sbit  LCD_PSB=P3^2;
uchar code table1[]={"**˫��ģ��**"};      //����Ҫ��ʾ���ַ��� 
uchar code table2[]={"�ٶ�--·��--ʱ��"}; 
/**************************************/
//��ʱ����
/**************************************/
void Delay_ms(uchar n)
{
   uchar i;
   uchar j;
   for(i=0;i<n;i++)
   for(j=0;j<125;j++)        //���1ms
 _nop_();
}
/**************************************/
//���з���һ���ֽ�
/**************************************/
void LCD_sendbyte(uchar byte)
{
   uchar i;
   for(i=0;i<8;i++)
    {
   LCD_SCLK=0;               //����ʱ����
   _nop_();
   LCD_SID=(bit)(byte&0x80);    //�������λ����
   LCD_SCLK=1;               //�����ط�������
   byte=byte<<1;                  //����һλ
 }
}

/****************************************/
//дָ��
/****************************************/
void LCD_write_com(uchar com)
{
  LCD_CS=1;        
   LCD_sendbyte(0xf8);       //����5�������ġ�1��������һ������,11111,RW(0),RS(0),0
   LCD_sendbyte(0xf0&com);       //ȡ����λ�����ݷ����δ��ͣ�
                                 //ÿ���ֽڵ����ݱ����������ֽ�
               //����λ���ڵ�һ���ֽڵĸ���λ
   LCD_sendbyte(0xf0&(com<<4));        //����λ���ڵڶ����ֽڵĸ���λ 
    LCD_CS=0;       
  Delay_ms(10);             //���в�֧�ֶ����������ɼ��æ��������������ʱ���   
}
/******************************************/
//д����
/******************************************/
void LCD_write_dat(uchar dat)
{
    LCD_CS=1;        
   LCD_sendbyte(0xfa);        //����5�������ġ�1��������һ������,11111,RW(0),RS(1),0
   LCD_sendbyte(0xf0&dat);       //ȡ����λ�����ݷ����δ��ͣ�
                                 //ÿ���ֽڵ����ݱ����������ֽ�
               //����λ���ڵ�һ���ֽڵĸ���λ
   LCD_sendbyte(0xf0&(dat<<4));        //����λ���ڵڶ����ֽ�
    LCD_CS=0;        
   Delay_ms(10);
}
/********************************************/
//LCD��ʼ��
/********************************************/
void LCD_init(void)
{  
 Delay_ms(5);
 LCD_RST=1;
 LCD_RST=0;
 LCD_RST=1;
 LCD_PSB=0;	
 LCD_write_com(0x30);             //ѡ�����ָ�   
 LCD_write_com(0x0c);       //����ʾ�����α꣬������
 LCD_write_com(0x01);       //�����ʾ��Ļ����DDRAMλַ����������Ϊ00H
 Delay_ms(5);         //��������ʱ��ϳ�1.6ms ��˼Ӵ���ʱ
 LCD_write_com(0x02);       //��DDRAMλַ��λ,�˴�ò���������ظ�
 LCD_write_com(0x06);       //�趨������ƣ�������ʾ���ƶ�
}

/***************************************************/
//������ʾ��ַ
/***************************************************/
void LCD_Setaddress(uchar x,uchar y)
{              //��ַ�ӵ�1�е�1�п�ʼ����0��ʼ
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
   LCD_write_com(addr);    //�ַ���ʾ��ʼ��ַ
}
/*****************************************************/
//���ַ�����ʾ�ڹ̶�λ��
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
		LCD_Putstring(x,y,"��ʱ");
 
 
}


void LCD_Putfloatv( uchar x, uchar y, float z  )
{
	uchar z1,z2;
	z1=(uchar)z;
	z2=(z-z1)*10;
	if(z>=10)
	{
		LCD_Putstring(x,y,"����");
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
		LCD_Putstring(x,y,"����");
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
