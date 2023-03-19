#include "S51Kernel.h"
#include "DigitronDisPlay.h"
#include "LEDDisPlay.h"
#include <stdlib.h>
#include "Tetris.h"
#include "TetrisData.h"
#include "TimerMain.h"
extern uchar MainState;
void TetrisDataWrite();
void TetrisDataRemove();
uchar TetrisTestBuff();
void Tetriscross(char Direction);
void TetrisRotate();
void TetrisClearRow();
void TetrisEnd();
TimerCallBackInt TetrisCallBack={
10000,10000,Tetris_TimerCallBack,NULL};
uchar TetrisGameState=0;
uchar GameData[12]=0;
uchar *GameMap=&GameData[4];
uchar GameScore=0;
char FallingBlockX,FallingBlockY;
uchar FallingType,NextType;
//0:未运行，1:运行中,2:失败,3:暂停
void InitTetris(){
	LEDSetBuff(&OpenData);
	LEDSwapBuff();
}
void TetrisWin(){
	TetrisEnd();
}
void TetrisEnd(){
	uchar i;
	MainState=0;
	DelTimerINT(&TetrisCallBack);
	for(i=0;i!=12;++i)
		GameData[i]=0;
    InitTetris();
	DIGData[Dig_ScoreStart]=0;
	DIGData[Dig_ScoreStart+1]=0;
	DIGData[Dig_Hint]=0;
	GameScore=0;
  TetrisGameState=0;	
}
void TetrisScore(){
	uchar i,j,LineClear=0;
	for(i=11,j=11;i<250;--i,--j){
		while(GameData[j]==0xFF){
			--j;
			OpenBuzzer(BuzzerTime);
			++LineClear;
			GameScore+=(DEFINE_Tetris_Score_Func(LineClear));
			if(GameScore>=Tetris_Score) {
				TetrisWin();
				return;
			}else{
				DIGData[Dig_ScoreStart]=DIG_CODE[GameScore%10];
				DIGData[Dig_ScoreStart+1]=DIG_CODE[GameScore/10];
			}
			if(j>250) break;
		}
		if(j>250) break;
		GameData[i]=GameData[j];	
	}	
}
void TetrisPause(){
	if(TetrisGameState==1){
		DelTimerINT(&TetrisCallBack);
		TetrisGameState=3;
	}else if(TetrisGameState==3){
		AddTimerINT(&TetrisCallBack);
		TetrisGameState=1;		
	}
}
void Tetris_TimerCallBack(){	
	TetrisDataRemove();
	++FallingBlockY;
	if((TetrisTestBuff())&&(FallingBlockY!=9)){
		TetrisDataWrite();
	}else{
		--FallingBlockY;
		TetrisDataWrite();
		TetrisScore();
		if(!(GameData[0]|GameData[1]|GameData[2]|GameData[3])){
			FallingType=NextType;
			NextType=(TimerRNDNum%28)<<1;
			DIGData[Dig_Hint]=HintData[NextType/8];
			FallingBlockY=0;
			FallingBlockX=2;
			TetrisDataWrite();
		}else{
			TetrisGameState=2;
			TetrisEnd();
			return;
		}
	}
	LEDSetBuff(GameMap);
	LEDSwapBuff();	
}
void TetrisQuickDown(){
	TetrisDataRemove();
	++FallingBlockY;
	while(TetrisTestBuff()&&(FallingBlockY!=9))	++FallingBlockY;
	--FallingBlockY;
	TetrisDataWrite();
	LEDSetBuff(GameMap);
	LEDSwapBuff();
}
void Tetriscross(char Direction){
	TetrisDataRemove();
	FallingBlockX+=Direction;
		if(TetrisTestBuff()){
			TetrisDataWrite();
			LEDSetBuff(GameMap);
			LEDSwapBuff();
		}else{
			FallingBlockX-=Direction;
			TetrisDataWrite();
		}					
}
void TetrisRotate(){
	uchar OldType;
	OldType=FallingType;
	TetrisDataRemove();
	if(FallingType%8==6)
		FallingType-=6;
	else 
		FallingType+=2;	
	if(TetrisTestBuff()){
		TetrisDataWrite();
		LEDSetBuff(GameMap);
		LEDSwapBuff();
	}else{
		FallingType=OldType;
		TetrisDataWrite();
	}

}
uchar TetrisTestBuff(){
	if(FallingBlockX>=0)
		if(FallingBlockX<5)
			return
			!((GameData[FallingBlockY  ]&(CHARH4BITL(BlockData[FallingType])<<FallingBlockX))
			||(GameData[FallingBlockY+1]&(CHARL4BIT (BlockData[FallingType])<<FallingBlockX))
			||(GameData[FallingBlockY+2]&(CHARH4BITL(BlockData[FallingType+1])<<FallingBlockX))
			||(GameData[FallingBlockY+3]&(CHARL4BIT (BlockData[FallingType+1])<<FallingBlockX)));
		else
			return
			!((GameData[FallingBlockY  ]&(CHARH4BITL(BlockData[FallingType  ])<<FallingBlockX))
			||(GameData[FallingBlockY+1]&(CHARL4BIT (BlockData[FallingType  ])<<FallingBlockX))
			||(GameData[FallingBlockY+2]&(CHARH4BITL(BlockData[FallingType+1])<<FallingBlockX))
			||(GameData[FallingBlockY+3]&(CHARL4BIT (BlockData[FallingType+1])<<FallingBlockX))
			||(CHARHBIT   (BlockData[FallingType  ],(FallingBlockX-4)))
			||(CHARATOBBIT(BlockData[FallingType  ],(8-FallingBlockX),3))
			||(CHARHBIT   (BlockData[FallingType+1],(FallingBlockX-4)))
			||(CHARATOBBIT(BlockData[FallingType+1],(8-FallingBlockX),3))
			);

	else
		return
		!((GameData[FallingBlockY  ]&(CHARH4BITL(BlockData[FallingType  ])>>(-FallingBlockX)))
		||(GameData[FallingBlockY+1]&(CHARL4BIT (BlockData[FallingType  ])>>(-FallingBlockX)))
    ||(GameData[FallingBlockY+2]&(CHARH4BITL(BlockData[FallingType+1])>>(-FallingBlockX)))
    ||(GameData[FallingBlockY+3]&(CHARL4BIT (BlockData[FallingType+1])>>(-FallingBlockX)))
		||(CHARATOBBIT(BlockData[FallingType  ],4,(3-FallingBlockX)))
		||(CHARLBIT   (BlockData[FallingType  ],(-FallingBlockX)))
		||(CHARATOBBIT(BlockData[FallingType+1],4,(3-FallingBlockX)))
		||(CHARLBIT   (BlockData[FallingType+1],(-FallingBlockX)))
		);
}
void TetrisDataWrite(){
	if(FallingBlockX>=0){
		GameData[FallingBlockY  ]|= CHARH4BITL(BlockData[FallingType])<<FallingBlockX;
		GameData[FallingBlockY+1]|= CHARL4BIT (BlockData[FallingType])<<FallingBlockX;
		GameData[FallingBlockY+2]|= CHARH4BITL(BlockData[FallingType+1])<<FallingBlockX;
		GameData[FallingBlockY+3]|= CHARL4BIT (BlockData[FallingType+1])<<FallingBlockX;
	}else{
		GameData[FallingBlockY  ]|= CHARH4BITL(BlockData[FallingType])>>(-FallingBlockX);
		GameData[FallingBlockY+1]|= CHARL4BIT (BlockData[FallingType])>>(-FallingBlockX);
		GameData[FallingBlockY+2]|= CHARH4BITL(BlockData[FallingType+1])>>(-FallingBlockX);
		GameData[FallingBlockY+3]|= CHARL4BIT (BlockData[FallingType+1])>>(-FallingBlockX);
	}
}
void TetrisDataRemove(){
	if(FallingBlockX>=0){
		GameData[FallingBlockY  ]^= CHARH4BITL(BlockData[FallingType])<<FallingBlockX;
		GameData[FallingBlockY+1]^= CHARL4BIT (BlockData[FallingType])<<FallingBlockX;
		GameData[FallingBlockY+2]^= CHARH4BITL(BlockData[FallingType+1])<<FallingBlockX;
		GameData[FallingBlockY+3]^= CHARL4BIT (BlockData[FallingType+1])<<FallingBlockX;
	}else{
		GameData[FallingBlockY  ]^= CHARH4BITL(BlockData[FallingType])>>(-FallingBlockX);
		GameData[FallingBlockY+1]^= CHARL4BIT (BlockData[FallingType])>>(-FallingBlockX);
		GameData[FallingBlockY+2]^= CHARH4BITL(BlockData[FallingType+1])>>(-FallingBlockX);
		GameData[FallingBlockY+3]^= CHARL4BIT (BlockData[FallingType+1])>>(-FallingBlockX);
	}
}
void TetrisStart(){
	TetrisCallBack.TimeRemaining=10000;
	TetrisGameState=1;
	AddTimerINT(&TetrisCallBack);
	FallingType=(TimerRNDNum%28)<<1;
	NextType=((FallingType+23)%28)<<1;
	FallingBlockY=0;
	FallingBlockX=2;
	TetrisDataWrite();
	DIGData[Dig_Hint]=HintData[NextType/8];
	DIGData[Dig_ScoreStart]=DIG_CODE[0];
	DIGData[Dig_ScoreStart+1]=DIG_CODE[0];
	LEDSetBuff(GameMap);
	LEDSwapBuff();
}
void KeyPress(uchar KeyValue){
	switch(KeyValue){
	case Key_Start:if(TetrisGameState==0) TetrisStart();break;
	case Key_Left :if(TetrisGameState==1) Tetriscross(1);break;
	case Key_Right:if(TetrisGameState==1) Tetriscross(-1);break;
	case Key_Rotate:if(TetrisGameState==1)TetrisRotate();break;
	case Key_QuickDown:if(TetrisGameState==1)TetrisQuickDown();break;
	case Key_End:if(TetrisGameState==1) TetrisEnd();break;
	case Key_Pause:if((TetrisGameState==1)||(TetrisGameState==3)) TetrisPause();break;
	}
}