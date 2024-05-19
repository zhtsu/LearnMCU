#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"

#define WHEN_BUTTON_PRESSED(Button) \
	DelayMs(20); 					\
	while(Button == 0); 			\
	DelayMs(20);

unsigned char MatrixKey()
{
	unsigned char KeyNum = 0;
	
	P1 = 0xFF;
	P1_3 = 0;
	if (P1_7 == 0) { WHEN_BUTTON_PRESSED(P1_7); KeyNum = 1; }
	if (P1_6 == 0) { WHEN_BUTTON_PRESSED(P1_6); KeyNum = 5; }
	if (P1_5 == 0) { WHEN_BUTTON_PRESSED(P1_5); KeyNum = 9; }
	if (P1_4 == 0) { WHEN_BUTTON_PRESSED(P1_4); KeyNum = 13; }
	
	P1 = 0xFF;
	P1_2 = 0;
	if (P1_7 == 0) { WHEN_BUTTON_PRESSED(P1_7); KeyNum = 2; }
	if (P1_6 == 0) { WHEN_BUTTON_PRESSED(P1_6); KeyNum = 6; }
	if (P1_5 == 0) { WHEN_BUTTON_PRESSED(P1_5); KeyNum = 10; }
	if (P1_4 == 0) { WHEN_BUTTON_PRESSED(P1_4); KeyNum = 14; }
	
	P1 = 0xFF;
	P1_1 = 0;
	if (P1_7 == 0) { WHEN_BUTTON_PRESSED(P1_7); KeyNum = 3; }
	if (P1_6 == 0) { WHEN_BUTTON_PRESSED(P1_6); KeyNum = 7; }
	if (P1_5 == 0) { WHEN_BUTTON_PRESSED(P1_5); KeyNum = 11; }
	if (P1_4 == 0) { WHEN_BUTTON_PRESSED(P1_4); KeyNum = 15; }
	
	P1 = 0xFF;
	P1_0 = 0;
	if (P1_7 == 0) { WHEN_BUTTON_PRESSED(P1_7); KeyNum = 4; }
	if (P1_6 == 0) { WHEN_BUTTON_PRESSED(P1_6); KeyNum = 8; }
	if (P1_5 == 0) { WHEN_BUTTON_PRESSED(P1_5); KeyNum = 12; }
	if (P1_4 == 0) { WHEN_BUTTON_PRESSED(P1_4); KeyNum = 16; }
	
	return KeyNum;
}

unsigned char KeyNum = 0;
unsigned int Password = 0;
unsigned char Count = 0;

void ResetPassword()
{
	Password = 0;
	Count = 0;
	LCD_ShowNum(2, 1, Password, 4);
}

void main()
{
	LCD_Init();
	LCD_ShowString(1, 1, "Password:");
	while(1)
	{
		KeyNum = MatrixKey();
		
		if (KeyNum == 0) continue;
		
		if (KeyNum == 11)
		{
			if (Password == 1234)
			{
				LCD_ShowString(1, 14, "OK ");
				ResetPassword();
			}
			else
			{
				LCD_ShowString(1, 14, "ERR");
				ResetPassword();
			}
		}
		else if (KeyNum == 12)
		{
			ResetPassword();
			LCD_ShowString(1, 14, "   ");
		}
		
		if (KeyNum == 0 || KeyNum > 10 || Count == 4)
		{
			continue;
		}
		
		Password *= 10;
		Password += KeyNum % 10;
		Count++;
		
		LCD_ShowNum(2, 1, Password, 4);
	}
}
