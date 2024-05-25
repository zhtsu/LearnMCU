#ifndef __MUSIC_H__
#define __MUSIC_H__

#define P	0
#define L1	1
#define L2	3
#define L3	5
#define L4	6
#define L5	8
#define L6	10
#define L7	12

#define M1	13
#define M2	15
#define M3	17
#define M4	18
#define M5	20
#define M6	22
#define M7	24

#define H1	25
#define H2	27
#define H3	29
#define H4	30
#define H5	32
#define H6	34
#define H7	36

unsigned char GetNixieLocation(unsigned char Syllable)
{
	switch (Syllable)
	{
		case 1: case 13: case 25: return 1; break;
		case 3: case 15: case 27: return 2; break;
		case 5: case 17: case 29: return 3; break;
		case 6: case 18: case 30: return 4; break;
		case 8: case 20: case 32: return 5; break;
		case 10: case 22: case 34: return 6; break;
		case 12: case 24: case 36: return 7; break;
	}
	
	return 0;
}

typedef struct
{
	unsigned char Syllable;
	unsigned char Duration;
} Note;

unsigned int code FreqTable[] = 
{
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283
};

Note code Music[] = 
{
	{H1, 2},{H2, 2},{H3, 2},{H5, 2},{H2, 4},{M6, 2},{M7, 2},
	{H1, 2},{H2, 2},{H3, 2},{H4, 2},{H3, 4},{P, 4},
	
	{H3, 2},{H2, 2},{H3, 2},{H5, 2},{H2, 4},{M6, 2},{M7, 2},
	{H1, 2},{M7, 2},{M6, 2},{M5, 2},{M3, 4},{P, 4},
	
	{H1, 2},{H2, 2},{H3, 2},{H5, 2},{H2, 4},{M6, 2},{M7, 2},
	{H1, 2},{H2, 2},{H3, 2},{H4, 2},{H3, 4},{P, 4},
	
	{H3, 2},{H1, 2},{H1, 2},{H3, 2},{H2, 4},{M7, 4},
	{H1, 2},{M7, 2},{M6, 4},{P, 4},
	
	{M1, 2},{M2, 2},{M3, 2},{M5, 2},{M2, 4},{L6, 2},{L7, 2},
	{M1, 2},{M2, 2},{M3, 2},{M4, 2},{M3, 4},{P, 4},
	
	{M3, 2},{M2, 2},{M3, 2},{M5, 2},{M2, 4},{L6, 2},{L7, 2},
	{M1, 2},{L7, 2},{L6, 2},{L5, 2},{L3, 4},{P, 4},
	
	{M1, 2},{M2, 2},{M3, 2},{M5, 2},{M2, 4},{L6, 2},{L7, 2},
	{M1, 2},{M2, 2},{M3, 2},{M4, 2},{M3, 4},{P, 4},
	
	{M3, 2},{M1, 2},{M1, 2},{M3, 2},{M2, 4},{L7, 4},
	{M1, 2},{L7, 2},{L6, 4},{P, 4},

	{M6, 2},{M3, 2},{M3, 2},{M5, 2},{M2, 4},
	{M1, 2},{M2, 2},{M3, 2},{M4, 2},{M3, 4},{P, 4},
	
	{M6, 2},{M3, 2},{M3, 2},{M5, 2},{M2, 4},
	{M1, 2},{M5, 2},{M3, 2},{M4, 2},{M3, 4},{P, 4},
	
	{M6, 2},{M3, 2},{M3, 2},{M5, 2},{H2, 4},
	{H1, 2},{H2, 2},{H3, 2},{M7, 2},{H1, 4},{P, 4},
	
	{H4, 2},{H3, 2},{H1, 2},{M6, 2},{M4, 2},{M3, 2},{M1, 2},{L6, 2},
	{M1, 4},{M2, 4},{M1, 4},
	
	{0xFF, 0xFF}
};

#endif
