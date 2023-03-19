#ifndef Simba_TIMERMAIN_H
#define Simba_TIMERMAIN_H
#ifdef __cplusplus
extern "C" {
#endif
typedef  void (code *VoidVoidFuncADR)(void);
typedef xdata struct TCBI{
	unsigned int TimeToCallBack;//单位是百微秒，即10代表每毫秒调用一次。
	unsigned int TimeRemaining;
	//若不定义宏TimerConfig_AutoIntiRemaining则该参数需要自行初始化！
	VoidVoidFuncADR CallBackFuncADR;
	struct TCBI *next;//private成员	
}TimerCallBackInt;

extern volatile unsigned int TimerRNDNum;
//extern volatile unsigned char MainTimerINTNum;
extern void InitMainTimer();
extern void SetTimerState(unsigned char state);
extern void RunTimerCallBack();
extern void AddTimerINT(TimerCallBackInt *AddINT);
extern char DelTimerINT(TimerCallBackInt *DelINT);
/*DelTimer返回值表
	0:成功删除计时器回调
	-1:未找到该计时器回调,uchar,255
	1:传入了NULL指针
*/
#ifdef __cplusplus
}
#endif
#endif
