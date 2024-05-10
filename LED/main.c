#include <REGX52.H>
#include <INTRINS.H>

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
	while (1)
	{
		if (P2 == 0x00)
		{
			P2 = 0xFE >> 1;
		}
			
		P2 = P2 << 1;
		DelayMs(500);
	}
}
	