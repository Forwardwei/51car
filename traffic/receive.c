#include "nRF24L01.h"
#include "def.h"
#include <stdio.h>
#include "QC12864B.h"
void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}

void main(void)
{
  float RxBuf[16];
	initprintf();
	init_io() ;
	LCDinit();
	 while(1)
	 {
		  if(nRF24L01_RxPacket(RxBuf))
		  {
				ES=0;
				TI=1;
				if(H==64)
				{
					printf("通道0000:%x ",H);
					printf("第1:%f   ",RxBuf[0]);
					printf("第2:%f    ",RxBuf[1]);
					printf("第3:%f    \n",RxBuf[2]);
					receiveA(RxBuf[0],RxBuf[1],RxBuf[2]);
				}
				if(H==66)
				{
					printf("通道1:%d ",H);
					printf("第1:%f   ",RxBuf[0]);
					printf("第2:%f    ",RxBuf[1]);
					printf("第3:%f    \n",RxBuf[2]);
					receiveB(RxBuf[0],RxBuf[1],RxBuf[2]);
				}
			TI=0;
			ES=1;
		  }
	}
}
		
	
	



