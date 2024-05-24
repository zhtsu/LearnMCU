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

sbit DS1302_SCLK = P3 ^ 6;
sbit DS1302_IO = P3 ^ 4;
sbit DS1302_CE = P3 ^ 5;

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

void main()
{
	unsigned char Second;
	
	LCD_Init();
	InitDS1302();
	LCD_ShowString(1, 1, "  -  -  ");
	LCD_ShowString(2, 1, "  :  :  ");
	DS1302_SetTime();
	
	while (1)
	{
		DS1302_ReadTime();
		LCD_ShowNum(1, 1, DS1302_Time[0], 2);
		LCD_ShowNum(1, 4, DS1302_Time[1], 2);
		LCD_ShowNum(1, 7, DS1302_Time[2], 2);
		LCD_ShowNum(2, 1, DS1302_Time[3], 2);
		LCD_ShowNum(2, 4, DS1302_Time[4], 2);
		LCD_ShowNum(2, 7, DS1302_Time[5], 2);
	}
}
