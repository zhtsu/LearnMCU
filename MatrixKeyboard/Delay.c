
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