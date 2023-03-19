#include "S51Kernel.h"
#include "DisPlay.h"
#include "KeyScan.h"
#include "TimerMain.h"

#define IO_Key P3
#define IO_MatrixKey P1
#define KeyBuffMax 5
extern void KeyPressCallBack(uchar KeyValue); 
//KeyPressCallBack按键检测到后应该调用的函数宏
//并在此立刻声明

TimerCallBackInt KeyScanTimerCallBack;
uchar xdata ButtonBuff[KeyBuffMax]=0;
uchar ButtonBuffCBID=0;//指示已经被回调函数处理完的按键起始ID
uchar ButtonBuffBit[3]=0;//需要20位
//uchar xdata * data ButtonBuffFront,xdata * data ButtonBuffRear;
TimerCallBackInt KeyDebounceCallBack[KeyBuffMax];
TimerCallBackInt *KeyDebounceCallBackBuff[KeyBuffMax];
void InitKeyScan(){
	uchar i;
	KeyScanTimerCallBack.next=NULL;
	KeyScanTimerCallBack.CallBackFuncADR=KeyScan_TimerCallBack;
	KeyScanTimerCallBack.TimeToCallBack=KeyScanSpeed;//KeyScanSpeed是宏
	KeyScanTimerCallBack.TimeRemaining=KeyScanSpeed;
	AddTimerINT(&KeyScanTimerCallBack);
	for(i=0;i!=KeyBuffMax;i++){
	   KeyDebounceCallBack[i].CallBackFuncADR=KeyDebounce_TimerCallBack;
	   KeyDebounceCallBack[i].TimeToCallBack=KeyDebounceTime;
	   KeyDebounceCallBackBuff[i]=&KeyDebounceCallBack[i];
	}
}
void KeyAdd(uchar KeyValue){
	uchar i;
	TimerCallBackInt *TempKeyDebounceCallBack;
	BITSET(ButtonBuffBit,KeyValue);
	//移动队列，把0-3移到1-4
	TempKeyDebounceCallBack=KeyDebounceCallBackBuff[4];
	for(i=3;i<=4;i--) {//i<=4检查是否下溢出
		ButtonBuff[i+1]=ButtonBuff[i];
		KeyDebounceCallBackBuff[i+1]=KeyDebounceCallBackBuff[i];
	}
	KeyDebounceCallBackBuff[0]=TempKeyDebounceCallBack;
	//处理处理完的按键起始ID指示
	++ButtonBuffCBID;
	//注册回调函数
	ButtonBuff[0]=KeyValue;
	//为了效率，考虑到AddTimerINT会处理好next指针，不对next执行初始化操作
	//*(KeyDebounceCallBackBuff[0]).next=NULL;
	//*(KeyDebounceCallBackBuff[0]).CallBackFuncADR=KeyDebounce_TimerCallBack;
	//*(KeyDebounceCallBackBuff[0]).TimeToCallBack=KeyDebounceTime;
	//KeyDebounceTime是宏,以上皆不初始化
	(*KeyDebounceCallBackBuff[0]).TimeRemaining=KeyDebounceTime;
	AddTimerINT(KeyDebounceCallBackBuff[0]);
}
void KeyBuffRemove(uchar KeyValue){
	uchar i,j;
	TimerCallBackInt *TempKeyDebounceCallBack;
	for(i=ButtonBuffCBID;i!=KeyBuffMax;i++){
		if(ButtonBuff[i]==KeyValue){
		//如果回调完成区内，删！
			BITCLEAR(ButtonBuffBit,KeyValue);
			TempKeyDebounceCallBack=KeyDebounceCallBackBuff[i];
			for(j=i;j!=KeyBuffMax;j++){
				ButtonBuff[j]=ButtonBuff[j+1];
				KeyDebounceCallBackBuff[j]=KeyDebounceCallBackBuff[j+1];
			}
			ButtonBuff[KeyBuffMax-1]=0;
			KeyDebounceCallBackBuff[KeyBuffMax-1]=TempKeyDebounceCallBack;
			return;//也即break
		}
	}					
}
void KeyScan_TimerCallBack(){
	uchar KeyValue,Key;
	KeyValue=17;
	IO_Key=0x0F;
	Key=(IO_Key&0x0F)|0x10;//只取低四位,并设第五位为1
	while(Key!=1){
		if((Key%2)==0){
			if(!BITTEST(ButtonBuffBit,KeyValue)) 
				KeyAdd(KeyValue);		
			//证明没有在Buff区,开始处理		
		}else{
			if(BITTEST(ButtonBuffBit,KeyValue)) //如果在Buff区内
				KeyBuffRemove(KeyValue);			
		}
//直接开始下一轮循环~\(≧▽≦)/~啦啦啦
	KeyValue++;
	Key>>=1;
	}
	IO_MatrixKey=0x0F;
	if(IO_MatrixKey!=0x0f){//读取按键是否按下
		if(!BITTEST(ButtonBuffBit,1))  //不在Buff区
			KeyAdd(1);	
	}else{
		if(BITTEST(ButtonBuffBit,1)) //如果在Buff区内
			KeyBuffRemove(1);	
	}
}

void KeyDebounce_TimerCallBack(){
	uchar KeyValue,i;
	TimerCallBackInt *TempKeyDebounceCallBack;
	--ButtonBuffCBID;
	DelTimerINT(KeyDebounceCallBackBuff[ButtonBuffCBID]);//删除计时器回调
	KeyValue=ButtonBuff[ButtonBuffCBID];//保存KeyValue
	if(KeyValue>=17) {
		IO_Key|=0x0F;
		if((IO_Key>>(KeyValue-17))%2){//判断IO_Key的相应位是否为0
		//如果回调完成区内，删！
			TempKeyDebounceCallBack=KeyDebounceCallBackBuff[i];
			BITCLEAR(ButtonBuffBit,KeyValue);
			for(i=ButtonBuffCBID;i!=KeyBuffMax;i++){
				ButtonBuff[i]=ButtonBuff[i+1];
				KeyDebounceCallBackBuff[i]=KeyDebounceCallBackBuff[i+1];
			}
			ButtonBuff[KeyBuffMax-1]=0;
			KeyDebounceCallBackBuff[KeyBuffMax-1]=TempKeyDebounceCallBack;
		}else{
		//否则的话，按键已经被按下，发送按键消息
			KeyPressCallBack(KeyValue);	
		}	
	}else{
		//测试列
		IO_MatrixKey=0X0F;
		switch(IO_MatrixKey){
			case(0X07):	KeyValue=0;break;
			case(0X0b):	KeyValue=1;break;
			case(0X0d): KeyValue=2;break;
			case(0X0e):	KeyValue=3;break;
			default: KeyBuffRemove(1);return;//如果读取的列行不正确，则跳过回调调用，并丢弃此次消息
		}
		//测试行
		IO_MatrixKey=0XF0;
		switch(IO_MatrixKey){
			case(0X70):	break;
			case(0Xb0):	KeyValue+=4;break;
			case(0Xd0): KeyValue+=8;break;
			case(0Xe0):	KeyValue+=12;break;
			default: KeyBuffRemove(1);return;//如果读取的列行不正确，则跳过回调调用，并丢弃此次消息
		}
		KeyPressCallBack(KeyValue);		
	}
}