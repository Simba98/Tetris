#include <intrins.h>
#include "STDHeader.h"
#include "Sreg52.h"
#include "SimbaTypeDef.h"
void InitAll(){	
	InitMainTimer();//±ØÐë×îÏÈ×¢²á
	InitDisPlay();
	InitKeyScan();	
}
TimerCallBackInt TestCallB;
uchar test=0x01;
uchar LED[8]=
{
	0x01,0x02,0x04,0x08,
	0x10,0x20,0x40,0x80,
};
uchar code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
int num,num2;
void KeyPress(uchar KeyValue){
	if(KeyValue>=17)
		DIGData[5]=DIG_CODE[KeyValue-17];
	else
		DIGData[6]=DIG_CODE[KeyValue];		
}
void Test_TimerCallBack(){ 
	uchar i;
	for(i=0;i!=8;i++){
		LED[i]=_cror_(LED[i],1);
	}
	LEDSetBuff(&LED);
	LEDSwapBuff();
}	    
int main(){


	InitAll();
	LEDSetBuffLine(3,2);
	LEDSetBuff(&LED);
	LEDSwapBuff(); 
	TestCallB.next=0;
	TestCallB.CallBackFuncADR=Test_TimerCallBack;
	TestCallB.TimeToCallBack=10000;
	TestCallB.TimeRemaining=10000;
	AddTimerINT(&TestCallB);
	SetTimerState(3);
	
	DIGData[4]=0x5e;
	DIGData[7]=0x66;
	while(1){
		RunTimerCallBack();
	}
		
	return 0;		  	
}