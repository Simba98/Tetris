#include "S51Kernel.h"
#include "DisPlay.h"
#include "DigitronDisplay.h"
#include "LEDDisPlay.h"
#include "TimerMain.h"

TimerCallBackInt DisPlayTimerCallBack;
uchar DisLineNum=0;//From 0To7
void InitDisPlay(){
	DisPlayTimerCallBack.next=NULL;
	DisPlayTimerCallBack.CallBackFuncADR=DisPlay_TimerCallBack;
	DisPlayTimerCallBack.TimeToCallBack=DisPlayFlushSpeed;//DisPlayFlushSpeed是宏
	DisPlayTimerCallBack.TimeRemaining=DisPlayFlushSpeed;
	AddTimerINT(&DisPlayTimerCallBack);
}

void DisPlay_TimerCallBack(){
	if( (--DisLineNum) >=8)//检测下溢出，也即255
		DisLineNum=7;

	DigDis_TimerCallBack();
	LEDDis_TimerCallBack();
}