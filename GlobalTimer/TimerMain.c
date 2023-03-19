#include "S51Kernel.h"
#include "TimerMain.h"

volatile uint MainTimerINTNum;
volatile uint TimerRNDNum;
TimerCallBackInt *TimerTableHeader=0,*TimerTableTail=0;
void InitMainTimer(){
//��ʼ��T2��ʱ��
	T2CON = 0;
	T2MOD = 0;
	TL2 = RCAP2L = TL2_Start;		//���ö�ʱ��ֵ����ֵ
	TH2 = RCAP2H = TH2_Start;		//���ö�ʱ��ֵ����ֵ
	ET2=1;//T2�ж�����
	EA=1;
	//TR2=1;//T2������
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
		MainTimerINTNum=0;//�����
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
		//ͨ�������TimerConfig_AutoIntiRemaining��ʹ�Զ���ʼ����Ч
	}
}
/*DelTimer����ֵ��
	0:�ɹ�ɾ����ʱ���ص�
	-1:δ�ҵ��ü�ʱ���ص�,uchar,255
	1:������NULLָ��
*/
char DelTimerINT(TimerCallBackInt *DelINT){	
	if(DelINT){
		TimerCallBackInt *p;
		TimerCallBackInt *last;
		//���ɾ����ͷ�ڵ�
		p=TimerTableHeader;
		if(DelINT==TimerTableHeader){
			TimerTableHeader=TimerTableHeader->next;
		#ifndef TimerConfig_DisableDelClearNext
			p->next=NULL;//����Ҳ��
		#endif
			if(DelINT==TimerTableTail)//����Ǳ�β�ڵ�
				TimerTableTail=NULL;
			return 0;		
		}
		//������Ǳ�ͷ�ڵ�
		while((DelINT!=p)&&(p!=NULL)){
			last=p;
			p=p->next;
		}
		if(DelINT==p){
			last->next=p->next;
		#ifndef TimerConfig_DisableDelClearNext
			p->next=NULL;//����Ҳ��
		#endif
			if(DelINT==TimerTableTail)//����Ǳ�β�ڵ�
				TimerTableTail=last;		
			return 0;
		}
		if(p==NULL) return -1;//������-1��δ�ҵ�
	}
	return 1;//������NULLָ��
}
void TimerMainINT() interrupt 5{
	TF2=0;
	++MainTimerINTNum;
	++TimerRNDNum;
}