#include "S51Kernel.h"
#include "74H595.h"
#include "DisPlay.h"
#include "LEDDisPlay.h"

uchar xdata LEDBuffer[16]=0;
uchar xdata *data LEDBuffH=&LEDBuffer[0];
uchar xdata *data LEDBuffW=&LEDBuffer[8];
//H为当前显示区，W为写入缓冲区


/*LEDSetLine返回值表
	0:操作成功完成
	1:列号非法
*/
uchar LEDSetBuffLine(uchar dat,uchar num){
	char i;
	uchar value,bitcode=1;
	if(num>=8) return 1;
	for(i=0;i!=8;++i){
		value=dat>>7;
		dat<<=1;
		if (value)
        	LEDBuffW[i]|= bitcode;
    	else
        	LEDBuffW[i]&= ~bitcode;
		bitcode<<=1;
	}
	return 0;
}
void LEDSetBuff(uchar *dat){
	if(dat){
		uchar i;
		for(i=0;i!=8;++i){
			LEDBuffW[i]=dat[i];	
		}
	}
}
void LEDSwapBuff(){
	uchar xdata *data temp;
	uchar i;
	for(i=0;i!=8;++i)
		LEDBuffW[i]=~LEDBuffW[i];	
	temp=LEDBuffH;
	LEDBuffH=LEDBuffW;
	LEDBuffW=temp;		
}
void LEDCleanBuff(){
	char i;
	for(i=0;i!=8;++i){
		LEDBuffW[i]=0;	
	}
}
void LEDDis_TimerCallBack(){
	P0=0xff;
	Set595Byte(0x80>>DisLineNum);
	P0=(LEDBuffH[DisLineNum]);		
}