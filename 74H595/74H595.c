#include "S51Kernel.h"
#include "74H595.h"

sbit SRCLK=P3^6;
sbit RDCLK=P3^5;
sbit SER=P3^4;

void Set595Byte(uchar dat)
{
	uchar a;
	SRCLK=0;
	RDCLK=0;
	for(a=8;a!=0;--a)
	{
		SER=dat>>7;
		dat<<=1;
		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RDCLK=1;
	_nop_();
	_nop_();
	RDCLK=0;
}