#include <REGX52.H>
#include "music.h"

sbit Buzzer = P2 ^ 5;

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
	P2_4 = TubeTable[Location - 1].A;
	P2_3 = TubeTable[Location - 1].B;
	P2_2 = TubeTable[Location - 1].C;
	
	P0 = DigitTable[Number];
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

Note SelectedNote;
unsigned char NoteSelect;

void (*CallbackFunc)(void) = 0;

void Routine_Timer0() interrupt 1
{
	TH0 = FreqTable[SelectedNote.Syllable] / 256;
	TL0 = FreqTable[SelectedNote.Syllable] % 256;
	
	if (CallbackFunc) CallbackFunc();
}

void ActivateBuzzer()
{
	Buzzer = !Buzzer;
}

void main()
{
	DelayMs(1000);
	
	InitTimer0();
	
	CallbackFunc = ActivateBuzzer;
	
	while (1)
	{
		SelectedNote = Music[NoteSelect];
		if (SelectedNote.Syllable == 0xFF)
		{
			TR0 = 0;
			while (1);
		}
		
		if (SelectedNote.Syllable == 0)
		{
			TR0 = 0;
		}
		else
		{
			TR0 = 1;
			DisplayDigit(GetNixieLocation(SelectedNote.Syllable), 0);
		}
		
		DelayMs(SelectedNote.Duration * 240);
		
		NoteSelect++;
		
		TR0 = 0;
		DelayMs(5);
		TR0 = 1;
	}
}
