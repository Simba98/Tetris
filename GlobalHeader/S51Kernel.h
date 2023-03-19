#ifndef __SimbaC51KERNAL_H__
#define __SimbaC51KERNAL_H__

#include "Sreg52.h"
#include <intrins.h>
#include <limits.h>
#include "bitops.h"
#include "SimbaTypeDef.h"

#define DisPlayFlushSpeed 20
//ˢ��Ƶ�ʣ���λ�ǰ�΢�룬Ĭ����20
//Ҳ��2����ˢ��һ�Σ�ÿ2*8=16����һ��ˢ������
//ÿ��60֡
#define KeyScanSpeed 20
//ˢ��Ƶ�ʣ���λ�ǰ�΢�룬Ĭ����20
//Ҳ��2����ɨ��һ��
#define KeyDebounceTime 100
//��������ʱ������λ�ǰ�΢�룬Ĭ����100
//��10������ӳ�
#define KeyPressCallBack(KeyValue) KeyPress(KeyValue)
//KeyPressCallBack������⵽��Ӧ�õ��õĺ�����
#define TimerConfig_DisableDelClearNext
//��������ɾ��Timer�ص����Զ����nextָ��Ĺ���
//������ٶȽ����˰�ȫ��
//�����Ҫ�Ļ�ʹ��#undef����
#undef TimerConfig_AutoIntiRemaining
//undef��ʱ��û�������Զ���ʣ��ʱ���ʼ��
//�����������Զ����״�����ʱ��
//��Ϊ#define����Զ���ʼ��TimeRemainingΪTimeToCallBack
#define TL2_Start (0x38) 
#define TH2_Start (0xFF)
//�˴�ָʾ��T2��ʱ������װֵ���˴���24Mhz����Ƶ����100΢��
#define DIG_U2LE P1^0
#define DIG_LSA P2^2
#define DIG_LSB P2^3
#define DIG_LSC P2^4

#endif