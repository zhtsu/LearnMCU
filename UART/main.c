#include <REGX52.H>

#define RECEIVER

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

void InitUart()
{
#ifdef RECEIVER
	SCON = 0x50;
#endif
#ifndef RECEIVER
	SCON = 0x40;
#endif
	
	PCON &= 0x7F;
	
	// Init timer 1
	TMOD &= 0x0F;
	TMOD |= 0x20;
	TH1 = 0xFA;
	TL1 = 0xFA;
	ET1 = 0;
	TR1 = 1;
	
#ifdef RECEIVER
	EA = 1;
	ES = 1;
#endif
}

void SendByteByUart(unsigned char Byte)
{
	SBUF = Byte;
	while (TI == 0);
	TI = 0;
}

#ifndef RECEIVER

void main()
{
	unsigned char Sec = 0x00;
	
	InitUart();
	
	while (1)
	{
		SendByteByUart(Sec);
		Sec++;
		DelayMs(1000);
	}
}

#endif

#ifdef RECEIVER

void Routine_UART() interrupt 4
{
	if (RI == 1)
	{
		P2 = SBUF;
		SendByteByUart(SBUF);
		RI = 0;
	}
}

void main()
{
	InitUart();
	
	while (1)
	{
		
	}
}

#endif
