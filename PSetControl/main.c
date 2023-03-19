#include <intrins.h>
#include "STDHeader.h"
#include "Sreg52.h"
#include "SimbaTypeDef.h"
#include "Tetris.h"
uchar MainState=1;
void InitAll(){	
	InitMainTimer();//��������ע��
	InitDisPlay();
	InitKeyScan();
	InitTetris();	
	InitBuzzer();
}
	    
int main(){


	InitAll();
	SetTimerState(1);

//	TetrisStart();
	while(MainState){
		RunTimerCallBack();
	}
		
	return 0;		  	
}