#ifndef __SimbaC51KERNAL_H__
#define __SimbaC51KERNAL_H__

#include "Sreg52.h"
#include <intrins.h>
#include <limits.h>
#include "bitops.h"
#include "SimbaTypeDef.h"

#define DisPlayFlushSpeed 20
//刷新频率，单位是百微秒，默认是20
//也即2毫秒刷新一次，每2*8=16毫秒一个刷新周期
//每秒60帧
#define KeyScanSpeed 20
//刷新频率，单位是百微秒，默认是20
//也即2毫秒扫描一次
#define KeyDebounceTime 100
//消除抖动时长，单位是百微秒，默认是100
//即10毫秒的延迟
#define KeyPressCallBack(KeyValue) KeyPress(KeyValue)
//KeyPressCallBack按键检测到后应该调用的函数宏
#define TimerConfig_DisableDelClearNext
//禁用了在删除Timer回调后自动清除next指针的功能
//提高了速度降低了安全度
//如果需要的话使用#undef即可
#undef TimerConfig_AutoIntiRemaining
//undef的时候没有启用自动的剩余时间初始化
//这样允许了自定义首次运行时间
//改为#define则会自动初始化TimeRemaining为TimeToCallBack
#define TL2_Start (0x38) 
#define TH2_Start (0xFF)
//此处指示了T2定时器的重装值，此处在24Mhz的主频上是100微秒
#define DIG_U2LE P1^0
#define DIG_LSA P2^2
#define DIG_LSB P2^3
#define DIG_LSC P2^4

#endif