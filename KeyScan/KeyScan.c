#include "S51Kernel.h"
#include "DisPlay.h"
#include "KeyScan.h"
#include "TimerMain.h"

#define IO_Key P3
#define IO_MatrixKey P1
#define KeyBuffMax 5
extern void KeyPressCallBack(uchar KeyValue); 
//KeyPressCallBack������⵽��Ӧ�õ��õĺ�����
//���ڴ���������

TimerCallBackInt KeyScanTimerCallBack;
uchar xdata ButtonBuff[KeyBuffMax]=0;
uchar ButtonBuffCBID=0;//ָʾ�Ѿ����ص�����������İ�����ʼID
uchar ButtonBuffBit[3]=0;//��Ҫ20λ
//uchar xdata * data ButtonBuffFront,xdata * data ButtonBuffRear;
TimerCallBackInt KeyDebounceCallBack[KeyBuffMax];
TimerCallBackInt *KeyDebounceCallBackBuff[KeyBuffMax];
void InitKeyScan(){
	uchar i;
	KeyScanTimerCallBack.next=NULL;
	KeyScanTimerCallBack.CallBackFuncADR=KeyScan_TimerCallBack;
	KeyScanTimerCallBack.TimeToCallBack=KeyScanSpeed;//KeyScanSpeed�Ǻ�
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
	//�ƶ����У���0-3�Ƶ�1-4
	TempKeyDebounceCallBack=KeyDebounceCallBackBuff[4];
	for(i=3;i<=4;i--) {//i<=4����Ƿ������
		ButtonBuff[i+1]=ButtonBuff[i];
		KeyDebounceCallBackBuff[i+1]=KeyDebounceCallBackBuff[i];
	}
	KeyDebounceCallBackBuff[0]=TempKeyDebounceCallBack;
	//��������İ�����ʼIDָʾ
	++ButtonBuffCBID;
	//ע��ص�����
	ButtonBuff[0]=KeyValue;
	//Ϊ��Ч�ʣ����ǵ�AddTimerINT�ᴦ���nextָ�룬����nextִ�г�ʼ������
	//*(KeyDebounceCallBackBuff[0]).next=NULL;
	//*(KeyDebounceCallBackBuff[0]).CallBackFuncADR=KeyDebounce_TimerCallBack;
	//*(KeyDebounceCallBackBuff[0]).TimeToCallBack=KeyDebounceTime;
	//KeyDebounceTime�Ǻ�,���ϽԲ���ʼ��
	(*KeyDebounceCallBackBuff[0]).TimeRemaining=KeyDebounceTime;
	AddTimerINT(KeyDebounceCallBackBuff[0]);
}
void KeyBuffRemove(uchar KeyValue){
	uchar i,j;
	TimerCallBackInt *TempKeyDebounceCallBack;
	for(i=ButtonBuffCBID;i!=KeyBuffMax;i++){
		if(ButtonBuff[i]==KeyValue){
		//����ص�������ڣ�ɾ��
			BITCLEAR(ButtonBuffBit,KeyValue);
			TempKeyDebounceCallBack=KeyDebounceCallBackBuff[i];
			for(j=i;j!=KeyBuffMax;j++){
				ButtonBuff[j]=ButtonBuff[j+1];
				KeyDebounceCallBackBuff[j]=KeyDebounceCallBackBuff[j+1];
			}
			ButtonBuff[KeyBuffMax-1]=0;
			KeyDebounceCallBackBuff[KeyBuffMax-1]=TempKeyDebounceCallBack;
			return;//Ҳ��break
		}
	}					
}
void KeyScan_TimerCallBack(){
	uchar KeyValue,Key;
	KeyValue=17;
	IO_Key=0x0F;
	Key=(IO_Key&0x0F)|0x10;//ֻȡ����λ,�������λΪ1
	while(Key!=1){
		if((Key%2)==0){
			if(!BITTEST(ButtonBuffBit,KeyValue)) 
				KeyAdd(KeyValue);		
			//֤��û����Buff��,��ʼ����		
		}else{
			if(BITTEST(ButtonBuffBit,KeyValue)) //�����Buff����
				KeyBuffRemove(KeyValue);			
		}
//ֱ�ӿ�ʼ��һ��ѭ��~\(�R���Q)/~������
	KeyValue++;
	Key>>=1;
	}
	IO_MatrixKey=0x0F;
	if(IO_MatrixKey!=0x0f){//��ȡ�����Ƿ���
		if(!BITTEST(ButtonBuffBit,1))  //����Buff��
			KeyAdd(1);	
	}else{
		if(BITTEST(ButtonBuffBit,1)) //�����Buff����
			KeyBuffRemove(1);	
	}
}

void KeyDebounce_TimerCallBack(){
	uchar KeyValue,i;
	TimerCallBackInt *TempKeyDebounceCallBack;
	--ButtonBuffCBID;
	DelTimerINT(KeyDebounceCallBackBuff[ButtonBuffCBID]);//ɾ����ʱ���ص�
	KeyValue=ButtonBuff[ButtonBuffCBID];//����KeyValue
	if(KeyValue>=17) {
		IO_Key|=0x0F;
		if((IO_Key>>(KeyValue-17))%2){//�ж�IO_Key����Ӧλ�Ƿ�Ϊ0
		//����ص�������ڣ�ɾ��
			TempKeyDebounceCallBack=KeyDebounceCallBackBuff[i];
			BITCLEAR(ButtonBuffBit,KeyValue);
			for(i=ButtonBuffCBID;i!=KeyBuffMax;i++){
				ButtonBuff[i]=ButtonBuff[i+1];
				KeyDebounceCallBackBuff[i]=KeyDebounceCallBackBuff[i+1];
			}
			ButtonBuff[KeyBuffMax-1]=0;
			KeyDebounceCallBackBuff[KeyBuffMax-1]=TempKeyDebounceCallBack;
		}else{
		//����Ļ��������Ѿ������£����Ͱ�����Ϣ
			KeyPressCallBack(KeyValue);	
		}	
	}else{
		//������
		IO_MatrixKey=0X0F;
		switch(IO_MatrixKey){
			case(0X07):	KeyValue=0;break;
			case(0X0b):	KeyValue=1;break;
			case(0X0d): KeyValue=2;break;
			case(0X0e):	KeyValue=3;break;
			default: KeyBuffRemove(1);return;//�����ȡ�����в���ȷ���������ص����ã��������˴���Ϣ
		}
		//������
		IO_MatrixKey=0XF0;
		switch(IO_MatrixKey){
			case(0X70):	break;
			case(0Xb0):	KeyValue+=4;break;
			case(0Xd0): KeyValue+=8;break;
			case(0Xe0):	KeyValue+=12;break;
			default: KeyBuffRemove(1);return;//�����ȡ�����в���ȷ���������ص����ã��������˴���Ϣ
		}
		KeyPressCallBack(KeyValue);		
	}
}