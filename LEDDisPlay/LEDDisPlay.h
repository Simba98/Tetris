#ifndef Simba_LEDDISPLAY_H
#define Simba_LEDDISPLAY_H

#include "STDHeader.h"

extern void LEDDis_TimerCallBack();
extern unsigned char LEDSetBuffLine(unsigned char dat,unsigned char num);
/*LEDSetLine����ֵ��
	0:�����ɹ����
	1:�кŷǷ�
*/
extern void LEDSetBuff(unsigned char *dat);
extern void LEDCleanBuff();
extern unsigned char LEDSetBuffLine(unsigned char dat,unsigned char num);
//����д��LED����
extern void LEDSwapBuff();
extern unsigned char xdata *data LEDBuffH;
extern unsigned char xdata *data LEDBuffW;
//HΪ��ǰ��ʾ����WΪд�뻺����

#endif