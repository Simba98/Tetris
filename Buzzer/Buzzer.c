#include "Buzzer.h"
#include "S51Kernel.h"
#include "STDHeader.h"
#define BZ_On 0
#define BZ_Off 1
sbit BZ=P1^5;
TimerCallBackInt BuzzerTimerCallBack;
void Buzzer_TimerCallBack(){
	BZ=BZ_Off;
	DelTimerINT(&BuzzerTimerCallBack);	
}
void InitBuzzer(){
	BuzzerTimerCallBack.next=NULL;
	BuzzerTimerCallBack.CallBackFuncADR=Buzzer_TimerCallBack;
	BuzzerTimerCallBack.TimeToCallBack=BuzzerTime;//BuzzerTimeºê
	BuzzerTimerCallBack.TimeRemaining=BuzzerTime;
}
void OpenBuzzer(uint time){
	if(BZ==BZ_On){
		DelTimerINT(&BuzzerTimerCallBack);	
		BZ=BZ_On;
	}
	BuzzerTimerCallBack.TimeToCallBack=time;//BuzzerTimeºê
	BuzzerTimerCallBack.TimeRemaining=time;	
	AddTimerINT(&BuzzerTimerCallBack);	
		
}
