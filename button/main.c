#include <REGX52.H>

#define WHEN_BUTTON_PRESSED(Button) \
	DelayMs(20); 					\
	while(Button == 0); 			\
	DelayMs(20);

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

void main()
{
	unsigned char Step = 0;
	P2 = ~0x01;
	
	while (1)
	{
		if (P3_1 == 0)
		{
			WHEN_BUTTON_PRESSED(P3_1);
			
			if (Step == 7) Step = 0;
			else Step++;
			
			P2 = ~(0x01 << Step);
		}
		
		if (P3_0 == 0)
		{
			WHEN_BUTTON_PRESSED(P3_0);
			
			if (Step == 0) Step = 7;
			else Step--;
			
			P2 = ~(0x01 << Step);
		}
	}
}