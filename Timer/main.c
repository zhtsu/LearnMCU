#include <REGX52.H>
#define CLOCK
#ifndef CLOCK
	#include <INTRINS.H>
#endif
#ifdef CLOCK
	#include "LCD1602.h"
#endif

void InitTimer0()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	
	// TCON
	TF0 = 0;
	TR0 = 1;
	// 64535 / 256
	TH0 = 0xFC;
	// 64535 % 256
	TL0 = 0x66;
	
	// Interrupt system
	ET0 = 1;
	EA  = 1;
	PT0 = 0;
}

void (*CallbackFunc)(void) = 0;

void Routine_Timer0() interrupt 1
{
	static unsigned int Count = 0;
	
	TH0 = 0xFC;
	TL0 = 0x66;
	
	Count++;
	if (Count >= 1000)
	{
		Count = 0;
		if (CallbackFunc) CallbackFunc();
	}
}

#ifndef CLOCK

#define WHEN_BUTTON_PRESSED(Button) \
	DelayMs(20); 					\
	while(Button == 0); 			\
	DelayMs(20);

typedef enum
{
	LEFT,
	RIGHT
} LedMode;

// Delay for @11.0592MHz
void DelayMs(unsigned int xms)
{
	unsigned char i, j;
	
	while (xms--)
	{
		for (i = 2; i > 0; i--)
		{
			for (j = 199; j > 0; j--);
		}
	}
}

unsigned char Key()
{
	unsigned char KeyNum = 0;
	
	if (P3_1 == 0) { WHEN_BUTTON_PRESSED(P3_1); KeyNum = 1; }
	if (P3_0 == 0) { WHEN_BUTTON_PRESSED(P3_0); KeyNum = 2; }
	if (P3_2 == 0) { WHEN_BUTTON_PRESSED(P3_2); KeyNum = 3; }
	if (P3_3 == 0) { WHEN_BUTTON_PRESSED(P3_3); KeyNum = 4; }

	return KeyNum;
}

LedMode Mode = LEFT;

void WaterfallLight()
{
	if (Mode == LEFT) P2 = _crol_(P2, 1);
	else if (Mode == RIGHT) P2 = _cror_(P2, 1);
}

void main()
{
	unsigned char KeyNum = 0;
	
	P2 = 0xFE;
	InitTimer0();
	CallbackFunc = WaterfallLight;
	
	while (1)
	{
		KeyNum = Key();
		if (KeyNum)
		{
			if (KeyNum == 1) Mode = LEFT;
			if (KeyNum == 2) Mode = RIGHT;
		}
	}
}

#endif

#ifdef CLOCK

unsigned char Sec, Min, Hour;

void UpdateSec()
{
	Sec++;
	if (Sec > 60)
	{
		Sec = 0;
		Min++;
		if (Min > 60)
		{
			Min = 0;
			Hour++;
			if (Hour > 24) Hour = 0;
		}
	}
}

void main()
{
	LCD_Init();
	InitTimer0();
	CallbackFunc = UpdateSec;
	
	LCD_ShowString(1, 1, "Clock:");
	LCD_ShowString(2, 1, "  :  :");
	
	while (1)
	{
		LCD_ShowNum(2, 1, Hour, 2);
		LCD_ShowNum(2, 4, Min, 2);
		LCD_ShowNum(2, 7, Sec, 2);
	}
}

#endif
