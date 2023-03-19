#ifndef Simba_LEDDISPLAY_H
#define Simba_LEDDISPLAY_H

#include "STDHeader.h"

extern void LEDDis_TimerCallBack();
extern unsigned char LEDSetBuffLine(unsigned char dat,unsigned char num);
/*LEDSetLine返回值表
	0:操作成功完成
	1:列号非法
*/
extern void LEDSetBuff(unsigned char *dat);
extern void LEDCleanBuff();
extern unsigned char LEDSetBuffLine(unsigned char dat,unsigned char num);
//按行写入LED数据
extern void LEDSwapBuff();
extern unsigned char xdata *data LEDBuffH;
extern unsigned char xdata *data LEDBuffW;
//H为当前显示区，W为写入缓冲区

#endif