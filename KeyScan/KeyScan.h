#ifndef Simba_KEYSCAN_H
#define Simba_KEYSCAN_H

#include "STDHeader.h"

extern void InitKeyScan();
extern void KeyScan_TimerCallBack();
extern void KeyDebounce_TimerCallBack();

#endif