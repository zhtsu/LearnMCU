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

void DisplayDigit(unsigned char Location, Number)
{
	P2_4 = TubeTable[Location].A;
	P2_3 = TubeTable[Location].B;
	P2_2 = TubeTable[Location].C;
	
	P0 = DigitTable[Number];
}

void main()
{
	DisplayDigit(7, 2);
	
	while (1)
	{
		
	}
}
