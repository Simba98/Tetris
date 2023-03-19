#ifndef Simba_TIMERMAIN_H
#define Simba_TIMERMAIN_H
#ifdef __cplusplus
extern "C" {
#endif
typedef  void (code *VoidVoidFuncADR)(void);
typedef xdata struct TCBI{
	unsigned int TimeToCallBack;//��λ�ǰ�΢�룬��10����ÿ�������һ�Ρ�
	unsigned int TimeRemaining;
	//���������TimerConfig_AutoIntiRemaining��ò�����Ҫ���г�ʼ����
	VoidVoidFuncADR CallBackFuncADR;
	struct TCBI *next;//private��Ա	
}TimerCallBackInt;

extern volatile unsigned int TimerRNDNum;
//extern volatile unsigned char MainTimerINTNum;
extern void InitMainTimer();
extern void SetTimerState(unsigned char state);
extern void RunTimerCallBack();
extern void AddTimerINT(TimerCallBackInt *AddINT);
extern char DelTimerINT(TimerCallBackInt *DelINT);
/*DelTimer����ֵ��
	0:�ɹ�ɾ����ʱ���ص�
	-1:δ�ҵ��ü�ʱ���ص�,uchar,255
	1:������NULLָ��
*/
#ifdef __cplusplus
}
#endif
#endif
