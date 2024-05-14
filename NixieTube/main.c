#include <REGX52.H>

typedef struct
{
	int A;
	int B;
	int C;
} F74LS138;

F74LS138 TubeTable[] = {
	{1, 1, 1}, {1, 1, 0}, {1, 0, 1}, {1, 0, 0},
	{0, 1, 1}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}
};

unsigned char DigitTable[] = {
	0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7D, 0x7F, 0x6F
};

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

void DisplayDigit(unsigned char Location, Number)
{
	P2_4 = TubeTable[Location].A;
	P2_3 = TubeTable[Location].B;
	P2_2 = TubeTable[Location].C;
	
	P0 = DigitTable[Number];
	
	DelayMs(1);
	P0 = 0x00;
}

void main()
{
	while (1)
	{
		DisplayDigit(0, 1);
		DisplayDigit(1, 1);
		DisplayDigit(2, 4);
		DisplayDigit(4, 5);
		DisplayDigit(5, 1);
		DisplayDigit(6, 4);
	}
}
