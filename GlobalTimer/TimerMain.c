#include "S51Kernel.h"
#include "TimerMain.h"

volatile uint MainTimerINTNum;
volatile uint TimerRNDNum;
TimerCallBackInt *TimerTableHeader=0,*TimerTableTail=0;
void InitMainTimer(){
//初始化T2定时器
	T2CON = 0;
	T2MOD = 0;
	TL2 = RCAP2L = TL2_Start;		//设置定时初值重载值
	TH2 = RCAP2H = TH2_Start;		//设置定时初值重载值
	ET2=1;//T2中断允许
	EA=1;
	//TR2=1;//T2不启动
}
void SetTimerState(uchar state){
	TR2=(!(!state));
}
void RunTimerCallBack(){
	if(TimerTableHeader){                  //if(TimerTableHeader!=0)
		uint TimeNow;
		TimerCallBackInt *p,*t;
		p=TimerTableHeader;
		TimeNow=MainTimerINTNum;
		MainTimerINTNum=0;//清空了
		while(p){    //while next!=0
			t=p->next;
			if(TimeNow<(p->TimeRemaining)){
				p->TimeRemaining -= TimeNow;
			}else{
				p->TimeRemaining=p->TimeToCallBack;
				p->CallBackFuncADR();
			}
			p=t;	
	 	}
	}
}
void AddTimerINT(TimerCallBackInt *AddINT){
	if(TimerTableHeader==0){
		TimerTableHeader=TimerTableTail=AddINT;
		AddINT->next=NULL;
	}else{
		TimerTableTail->next=AddINT;
		AddINT->next=NULL;
		TimerTableTail=AddINT;
	#ifdef TimerConfig_AutoIntiRemaining
		TimerTableHeader->TimeRemaining=TimerTableHeader->TimeToCallBack;
	#endif
		//通过定义宏TimerConfig_AutoIntiRemaining来使自动初始化生效
	}
}
/*DelTimer返回值表
	0:成功删除计时器回调
	-1:未找到该计时器回调,uchar,255
	1:传入了NULL指针
*/
char DelTimerINT(TimerCallBackInt *DelINT){	
	if(DelINT){
		TimerCallBackInt *p;
		TimerCallBackInt *last;
		//如果删除表头节点
		p=TimerTableHeader;
		if(DelINT==TimerTableHeader){
			TimerTableHeader=TimerTableHeader->next;
		#ifndef TimerConfig_DisableDelClearNext
			p->next=NULL;//不清也可
		#endif
			if(DelINT==TimerTableTail)//如果是表尾节点
				TimerTableTail=NULL;
			return 0;		
		}
		//如果不是表头节点
		while((DelINT!=p)&&(p!=NULL)){
			last=p;
			p=p->next;
		}
		if(DelINT==p){
			last->next=p->next;
		#ifndef TimerConfig_DisableDelClearNext
			p->next=NULL;//不清也可
		#endif
			if(DelINT==TimerTableTail)//如果是表尾节点
				TimerTableTail=last;		
			return 0;
		}
		if(p==NULL) return -1;//错误码-1，未找到
	}
	return 1;//传递了NULL指针
}
void TimerMainINT() interrupt 5{
	TF2=0;
	++MainTimerINTNum;
	++TimerRNDNum;
}