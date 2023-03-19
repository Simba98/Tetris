#include "S51Kernel.h"
#include "74H595.h"
#include "DigitronDisPlay.h"
#include "DisPlay.h"
#include "TimerMain.h"
#define KeyMatrixPort P1
sbit U2LE=DIG_U2LE;
sbit LSA=DIG_LSA;
sbit LSB=DIG_LSB;
sbit LSC=DIG_LSC;

uchar xdata DIGData[8]=0;
//TimerCallBackInt DigTimerCallBack;

void DigDis_TimerCallBack(){
	uchar t;	
	t=DisLineNum;
	Set595Byte(0x00);//关闭LED点阵	
	KeyMatrixPort=0xFF;
	U2LE=1;
	P0=0;

	LSA=t%2;
	t=t>>1;
	LSB=t%2;
	t=t>>1;
	LSC=t%2;

	P0=DIGData[DisLineNum];
	_nop_();//延时以确保写入成功
	U2LE=0;	
}