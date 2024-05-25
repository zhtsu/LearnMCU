#include <REGX52.H>
#include "LCD1602.h"

#define DS1302_SECOND 	0x80
#define DS1302_MINUTE 	0x82
#define DS1302_HOUR 	0x84
#define DS1302_DATE 	0x86
#define DS1302_MONTH 	0x88
#define DS1302_DAY 		0x8A
#define DS1302_YEAR 	0x8C
#define DS1302_WP 		0x8E

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

#define WHEN_BUTTON_PRESSED(Button) \
	DelayMs(20); 					\
	while(Button == 0); 			\
	DelayMs(20);

sbit DS1302_SCLK = P3 ^ 6;
sbit DS1302_IO = P3 ^ 4;
sbit DS1302_CE = P3 ^ 5;

unsigned char Key()
{
	unsigned char KeyNum = 0;
	
	if (P3_1 == 0) { WHEN_BUTTON_PRESSED(P3_1); KeyNum = 1; }
	if (P3_0 == 0) { WHEN_BUTTON_PRESSED(P3_0); KeyNum = 2; }
	if (P3_2 == 0) { WHEN_BUTTON_PRESSED(P3_2); KeyNum = 3; }
	if (P3_3 == 0) { WHEN_BUTTON_PRESSED(P3_3); KeyNum = 4; }

	return KeyNum;
}

void InitTimer0()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	
	TF0 = 0;
	TR0 = 1;
	TH0 = 0xFC;
	TL0 = 0x66;
	
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
	if (Count >= 500)
	{
		Count = 0;
		if (CallbackFunc) CallbackFunc();
	}
}

void InitDS1302(void)
{
	DS1302_CE = 0;
	DS1302_SCLK = 0;
}

void WriteByteToDS1302(unsigned char Command, Data)
{
	unsigned char i;
	
	DS1302_CE = 1;
	
	for (i = 0; i < 8; i++)
	{
		DS1302_IO = Command & (0x01 << i);
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	
	for (i = 0; i < 8; i++)
	{
		DS1302_IO = Data & (0x01 << i);
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	
	DS1302_CE = 0;
}

unsigned char ReadByteFromDS1302(unsigned char Command)
{
	unsigned char i, Data = 0x00;
	
	Command |= 0x01;
	DS1302_CE = 1;
	
	for (i = 0; i < 8; i++)
	{
		DS1302_IO = Command & (0x01 << i);
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
	
	for (i = 0; i < 8; i++)
	{
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
		if (DS1302_IO)
		{
			Data |= (0x01 << i);
		}
	}
	
	DS1302_CE = 0;
	DS1302_IO = 0;
	
	return Data;
}

unsigned char DS1302_Time[] = {19, 11, 16, 12, 59, 55, 6};

unsigned char EncodeBCD(unsigned char BCD)
{
	return BCD / 10 * 16 + BCD % 10;
}

unsigned char DecodeBCD(unsigned char BCD)
{
	return BCD / 16 * 10 + BCD % 16;
}

void DS1302_SetTime()
{
	WriteByteToDS1302(DS1302_WP, 0x00);
	WriteByteToDS1302(DS1302_YEAR, EncodeBCD(DS1302_Time[0]));
	WriteByteToDS1302(DS1302_MONTH, EncodeBCD(DS1302_Time[1]));
	WriteByteToDS1302(DS1302_DATE, EncodeBCD(DS1302_Time[2]));
	WriteByteToDS1302(DS1302_HOUR, EncodeBCD(DS1302_Time[3]));
	WriteByteToDS1302(DS1302_MINUTE, EncodeBCD(DS1302_Time[4]));
	WriteByteToDS1302(DS1302_SECOND, EncodeBCD(DS1302_Time[5]));
	WriteByteToDS1302(DS1302_DAY, EncodeBCD(DS1302_Time[6]));
	WriteByteToDS1302(DS1302_WP, 0x80);
}

void DS1302_ReadTime()
{
	DS1302_Time[0] = DecodeBCD(ReadByteFromDS1302(DS1302_YEAR));
	DS1302_Time[1] = DecodeBCD(ReadByteFromDS1302(DS1302_MONTH));
	DS1302_Time[2] = DecodeBCD(ReadByteFromDS1302(DS1302_DATE));
	DS1302_Time[3] = DecodeBCD(ReadByteFromDS1302(DS1302_HOUR));
	DS1302_Time[4] = DecodeBCD(ReadByteFromDS1302(DS1302_MINUTE));
	DS1302_Time[5] = DecodeBCD(ReadByteFromDS1302(DS1302_SECOND));
	DS1302_Time[6] = DecodeBCD(ReadByteFromDS1302(DS1302_DAY));
}

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;

void TimeShow()
{
	if (TimeSetSelect == 0 && TimeSetFlashFlag == 1 && MODE) LCD_ShowString(1, 1, "  ");
	else LCD_ShowNum(1, 1, DS1302_Time[0], 2);
	
	if (TimeSetSelect == 1 && TimeSetFlashFlag == 1 && MODE) LCD_ShowString(1, 4, "  ");
	else LCD_ShowNum(1, 4, DS1302_Time[1], 2);
	
	if (TimeSetSelect == 2 && TimeSetFlashFlag == 1 && MODE) LCD_ShowString(1, 7, "  ");
	else LCD_ShowNum(1, 7, DS1302_Time[2], 2);
	
	if (TimeSetSelect == 3 && TimeSetFlashFlag == 1 && MODE) LCD_ShowString(2, 1, "  ");
	else LCD_ShowNum(2, 1, DS1302_Time[3], 2);
	
	if (TimeSetSelect == 4 && TimeSetFlashFlag == 1 && MODE) LCD_ShowString(2, 4, "  ");
	else LCD_ShowNum(2, 4, DS1302_Time[4], 2);
	
	if (TimeSetSelect == 5 && TimeSetFlashFlag == 1 && MODE) LCD_ShowString(2, 7, "  ");
	else LCD_ShowNum(2, 7, DS1302_Time[5], 2);
}

void TimeSetFlash()
{
	TimeSetFlashFlag = !TimeSetFlashFlag;
}

void TimeSet()
{
	if (KeyNum == 2)
	{
		TimeSetSelect++;
		TimeSetSelect %= 6;
	}
	else if (KeyNum == 3)
	{
		DS1302_Time[TimeSetSelect]++;
	}
	else if (KeyNum == 4)
	{
		DS1302_Time[TimeSetSelect]--;
	}
	
	TimeShow();
}

void main()
{
	LCD_Init();
	InitDS1302();
	InitTimer0();
	LCD_ShowString(1, 1, "  -  -  ");
	LCD_ShowString(2, 1, "  :  :  ");
	
	DS1302_SetTime();
	CallbackFunc = TimeSetFlash;
	
	while (1)
	{
		KeyNum = Key();
		if (KeyNum == 1)
		{
			if (MODE == 0)
			{
				MODE = 1;
				TimeSetSelect = 0;
			}
			else if (MODE == 1)
			{
				MODE = 0;
				DS1302_SetTime();
			}
		}
		
		if (MODE == 0)
		{
			DS1302_ReadTime();
			TimeShow();
		}
		else if (MODE == 1)
		{
			TimeSet();
		}	
	}
}
