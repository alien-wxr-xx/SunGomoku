/*                     _______
 *  Gomoku software   / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * table.c - lookup table generation
 */

#include "table.h"
#include "pattern.h"
#include "mvlist.h"

// lookup tables
// global variables with external link
u8 nei[15 * 15][NEI_SIZE];
u8 pattab15[P15][PATTAB_NUM];
u8 pattab14[P14][PATTAB_NUM];
u8 pattab13[P13][PATTAB_NUM];
u8 pattab12[P12][PATTAB_NUM];
u8 pattab11[P11][PATTAB_NUM];
u8 pattab10[P10][PATTAB_NUM];
u8 pattab9[P9][PATTAB_NUM];
u8 pattab8[P8][PATTAB_NUM];
u8 pattab7[P7][PATTAB_NUM];
u8 pattab6[P6][PATTAB_NUM];
u8 pattab5[P5][PATTAB_NUM];

extern bool isForbidden;

// for pattern lookup
static u8 pattab_trans[26][2] = {
	{ LONG,   BLACK },
	{ FIVE,   BLACK },
	{ FREE4,  BLACK },
	{ DEAD4,  BLACK },
	{ FREE3a, BLACK },
	{ FREE3,  BLACK },
	{ DEAD3,  BLACK },
	{ FREE2a, BLACK },
	{ FREE2,  BLACK },
	{ DEAD2,  BLACK },
	{ FREE1a, BLACK },
	{ FREE1,  BLACK },
	{ DEAD1,  BLACK },
	{ LONG,   WHITE },
	{ FIVE,   WHITE },
	{ FREE4,  WHITE },
	{ DEAD4,  WHITE },
	{ FREE3a, WHITE },
	{ FREE3,  WHITE },
	{ DEAD3,  WHITE },
	{ FREE2a, WHITE },
	{ FREE2,  WHITE },
	{ DEAD2,  WHITE },
	{ FREE1a, WHITE },
	{ FREE1,  WHITE },
	{ DEAD1,  WHITE },
};

/*******************************************************************************
							Neighbor table generation
*******************************************************************************/
static u8 nei_helper[19][19];

static void nei_helper_init()
{
	int r, c;
	for(r = 0; r < 19; r++)
	{
		for(c = 0; c < 19; c++)
		{
			if(r == 0 || r == 1 || r == 17 || r == 18
				|| c == 0 || c == 1 || c == 17 || c == 18)
				nei_helper[r][c] = INVALID;
			else
				nei_helper[r][c] = (r - 2) * 15 + c - 2;
		}
	}
}

void nei_table_init()
{
	int nr, nc, ar, ac;
	int index, i, j;

	nei_helper_init();

	for(nr = 0; nr < 15; nr++)
	{
		for(nc = 0; nc < 15; nc++)
		{
			index = 0;
		
			// 2 discs away
			for(i = 2; i >= -2; i -= 2)	
			{
				for(j = 2; j >= -2; j -= 2)
				{
					ar = nr + 2 + i;
					ac = nc + 2 + j;
					if(nei_helper[ar][ac] != INVALID && !(i == 0 && j == 0))
						nei[nr * 15 + nc][index++] = nei_helper[ar][ac];
				}
			}
	
			// 1 disc away
			for(i = 1; i >= -1; i -= 1)	
			{
				for(j = 1; j >= -1; j -= 1)
				{
					ar = nr + 2 + i;
					ac = nc + 2 + j;
					if(nei_helper[ar][ac] != INVALID && !(i == 0 && j == 0))
						nei[nr * 15 + nc][index++] = nei_helper[ar][ac];
				}
			}
	
			// mark the rest as invalid
			if(index != NEI_SIZE)
				for(i = index; i < NEI_SIZE; i++)
					nei[nr * 15 + nc][i] = INVALID;
		}
	}
}

/*******************************************************************************
							Line count implementation
*******************************************************************************/
#define PAT_ROW		122

// helper structure
typedef struct {
	bool is;
	u8 macro;
	u8 mask;
} pat_t;

// helper tables
static pat_t lon[P6];
static pat_t five[P5];
static pat_t free4[P6];
static pat_t dead4[P5];
static pat_t free3[P6];
static pat_t dead3[P5];
static pat_t free2[P6];
static pat_t dead2[P5];
static pat_t free1[P6];
static pat_t dead1[P5];
static pat_t free3a[P7];
static pat_t free2a[P7];
static pat_t free1a[P7];
static pat_t d4d47[P7];
static pat_t d4d48[P8];
static pat_t d4d49[P9];
static pat_t d4b7[P7];
static pat_t d3b8[P8];
static pat_t d3b7[P7];
static pat_t b6[P6];

// helper array for generating lookup tables
// one row represents pattern length, index, type, color and mask
static u16 pat_tab[PAT_ROW][5] = 
{
	// 1-2
	{ 6, 364, LONG,  B6, 0x3f },		// 111111
	{ 6, 728, LONG,  W6, 0x3f },

	// 3-4
	{ 5, 121, FIVE,  B5, 0x1f },		// 11111
	{ 5, 242, FIVE,  W5, 0x1f },

	// 5-6
	{ 6, 120, FREE4, Bf4, 0x1e },		// 011110
	{ 6, 240, FREE4, Wf4, 0x1e },
	
	// 7-16
	{ 5, 120, DEAD4, Bd4, 0x1e },		// 11110
	{ 5, 240, DEAD4, Wd4, 0x1e },
	{ 5, 118, DEAD4, Bd4, 0x1d },		// 11101
	{ 5, 236, DEAD4, Wd4, 0x1d },
	{ 5, 112, DEAD4, Bd4, 0x1b },		// 11011
	{ 5, 224, DEAD4, Wd4, 0x1b },
	{ 5, 94,  DEAD4, Bd4, 0x17 },		// 10111
	{ 5, 188, DEAD4, Wd4, 0x17 },
	{ 5, 40,  DEAD4, Bd4, 0x0f },		// 01111
	{ 5, 80,  DEAD4, Wd4, 0x0f },

	// 17-24
	{ 6, 117, FREE3, Bf3, 0x1c },		// 011100
	{ 6, 234, FREE3, Wf3, 0x1c },
	{ 6, 111, FREE3, Bf3, 0x1a },		// 011010
	{ 6, 222, FREE3, Wf3, 0x1a },
	{ 6, 93,  FREE3, Bf3, 0x16 },		// 010110
	{ 6, 186, FREE3, Wf3, 0x16 },
	{ 6, 39,  FREE3, Bf3, 0x0e },		// 001110
	{ 6, 78,  FREE3, Wf3, 0x0e },

	// 25-44
	{ 5, 117, DEAD3, Bd3, 0x1c},		// 11100
	{ 5, 234, DEAD3, Wd3, 0x1c},
	{ 5, 111, DEAD3, Bd3, 0x1a},		// 11010
	{ 5, 222, DEAD3, Wd3, 0x1a},
	{ 5, 93,  DEAD3, Bd3, 0x16},		// 10110
	{ 5, 186, DEAD3, Wd3, 0x16},
	{ 5, 39,  DEAD3, Bd3, 0x0e},		// 01110
	{ 5, 78,  DEAD3, Wd3, 0x0e},
	{ 5, 109, DEAD3, Bd3, 0x19},		// 11001
	{ 5, 218, DEAD3, Wd3, 0x19},
	{ 5, 91,  DEAD3, Bd3, 0x15},		// 10101
	{ 5, 182, DEAD3, Wd3, 0x15},
	{ 5, 37,  DEAD3, Bd3, 0x0d},		// 01101
	{ 5, 74,  DEAD3, Wd3, 0x0d},
	{ 5, 85,  DEAD3, Bd3, 0x13},		// 10011
	{ 5, 170, DEAD3, Wd3, 0x13},
	{ 5, 31,  DEAD3, Bd3, 0x0b},		// 01011
	{ 5, 62,  DEAD3, Wd3, 0x0b},
	{ 5, 13,  DEAD3, Bd3, 0x07},		// 00111
	{ 5, 26,  DEAD3, Wd3, 0x07},

	// 45-56
	{ 6, 108, FREE2, Bf2, 0x18 },		// 011000
	{ 6, 216, FREE2, Wf2, 0x18 },
	{ 6, 90,  FREE2, Bf2, 0x14 },		// 010100
	{ 6, 180, FREE2, Wf2, 0x14 },
	{ 6, 36,  FREE2, Bf2, 0x0c },		// 001100
	{ 6, 72,  FREE2, Wf2, 0x0c },
	{ 6, 84,  FREE2, Bf2, 0x12 },		// 010010
	{ 6, 168, FREE2, Wf2, 0x12 },
	{ 6, 30,  FREE2, Bf2, 0x0a },		// 001010
	{ 6, 60,  FREE2, Wf2, 0x0a },
	{ 6, 12,  FREE2, Bf2, 0x06 },		// 000110
	{ 6, 24,  FREE2, Wf2, 0x06 },


	// 57-76
	{ 5, 4,   DEAD2, Bd2, 0x03 },		// 00011
	{ 5, 8,   DEAD2, Wd2, 0x03 },
	{ 5, 10,  DEAD2, Bd2, 0x05 },		// 00101
	{ 5, 20,  DEAD2, Wd2, 0x05 },
	{ 5, 28,  DEAD2, Bd2, 0x09 },		// 01001
	{ 5, 56,  DEAD2, Wd2, 0x09 },
	{ 5, 82,  DEAD2, Bd2, 0x11 },		// 10001
	{ 5, 164, DEAD2, Wd2, 0x11 },
	{ 5, 12,  DEAD2, Bd2, 0x06 },		// 00110
	{ 5, 24,  DEAD2, Wd2, 0x06 },
	{ 5, 30,  DEAD2, Bd2, 0x0a },		// 01010
	{ 5, 60,  DEAD2, Wd2, 0x0a },
	{ 5, 84,  DEAD2, Bd2, 0x12 },		// 10010
	{ 5, 168, DEAD2, Wd2, 0x12 },
	{ 5, 36,  DEAD2, Bd2, 0x0c },		// 01100
	{ 5, 72,  DEAD2, Wd2, 0x0c },
	{ 5, 90,  DEAD2, Bd2, 0x14 },		// 10100
	{ 5, 180, DEAD2, Wd2, 0x14 },
	{ 5, 108, DEAD2, Bd2, 0x18 },		// 11000
	{ 5, 216, DEAD2, Wd2, 0x18 },

	// 77-84
	{ 6, 3,   FREE1, Bf1, 0x02 },		// 000010
	{ 6, 6,   FREE1, Wf1, 0x02 },
	{ 6, 9,   FREE1, Bf1, 0x04 },		// 000100
	{ 6, 18,  FREE1, Wf1, 0x04 },
	{ 6, 27,  FREE1, Bf1, 0x08 },		// 001000
	{ 6, 54,  FREE1, Wf1, 0x08 },
	{ 6, 81,  FREE1, Bf1, 0x10 },		// 010000
	{ 6, 162, FREE1, Wf1, 0x10 },

	// 85-94
	{ 5, 1,   DEAD1, Bd1, 0x01 },		// 00001
	{ 5, 2,   DEAD1, Wd1, 0x01 },
	{ 5, 3,   DEAD1, Bd1, 0x02 },		// 00010
	{ 5, 6,   DEAD1, Wd1, 0x02 },
	{ 5, 9,   DEAD1, Bd1, 0x04 },		// 00100
	{ 5, 18,  DEAD1, Wd1, 0x04 },
	{ 5, 27,  DEAD1, Bd1, 0x08 },		// 01000
	{ 5, 54,  DEAD1, Wd1, 0x08 },
	{ 5, 81,  DEAD1, Bd1, 0x10 },		// 10000
	{ 5, 162, DEAD1, Wd1, 0x10 },

	// 95-96
	{ 7, 117, FREE3a, Bf3a, 0x1c },		// 0011100
	{ 7, 234, FREE3a, Wf3a, 0x1c },

	// 97-100
	{ 7, 108, FREE2a, Bf2a, 0x18 },		// 0011000
	{ 7, 216, FREE2a, Wf2a, 0x18 },
	{ 7, 36,  FREE2a, Bf2a, 0x0c },		// 0001100
	{ 7, 72,  FREE2a, Wf2a, 0x0c },

	// 101-102
	{ 7, 27,  FREE1a, Bf1a, 0x08 },		// 0001000
	{ 7, 54,  FREE1a, Wf1a, 0x08 },

	// 103-104
	{ 7, 847, DEAD4, Bd4, 0x5d },		// 1011101
	{ 7, 1694,DEAD4, Wd4, 0x5d },

	// 105-106
	{ 8, 3028,DEAD4, Bd4, 0xdb },		// 11011011
	{ 8, 6056,DEAD4, Wd4, 0xdb },

	// 107-108
	{ 9, 9571,DEAD4, Bd4, 0x1d7},		// 111010111
	{ 9,19142,DEAD4, Wd4, 0x1d7},

	// 109-110
	{ 7, 361, DEAD4, Bd4, 0x3c },		// 0111101
	{ 7, 849, DEAD4, Bd4, 0x1e },		// 1011110

	// 111-114
	{ 8, 355, DEAD3, Bd3, 0x3b },		// 00111011
	{ 8, 3033,DEAD3, Bd3, 0xdc },		// 11011100
	{ 8, 4726,DEAD3, Bd3, 0x38 },		// 20111001
	{ 8, 2306,DEAD3, Bd3, 0x1c },		// 10011102

	// 115-118
	{ 7, 822, DEAD3, Bd3, 0x16 },		// 1010110
	{ 7, 840, DEAD3, Bd3, 0x1a },		// 1011010
	{ 7, 280, DEAD3, Bd3, 0x2c },		// 0101101
	{ 7, 334, DEAD3, Bd3, 0x34 },		// 0110101

	// 119-122
	{ 6, 361, INVALID, INVALID, 0x3c },	// 111101
	{ 6, 355, INVALID, INVALID, 0x3b },	// 111011
	{ 6, 337, INVALID, INVALID, 0x27 },	// 110111
	{ 6, 283, INVALID, INVALID, 0x1e }	// 101111
};

// initialize pat_t arrays
static void pat_t_init()
{
	int i;

	// reset all
	for(i = 0; i < P5; i++)
	{
		five[i].is = false;
		dead4[i].is = false;
		dead3[i].is = false;
		dead2[i].is = false;
		dead1[i].is = false;
	}
	for(i = 0; i < P6; i++)
	{
		lon[i].is = false;
		free4[i].is = false;
		free3[i].is = false;
		free2[i].is = false;
		free1[i].is = false;
		b6[i].is = false;
	}
	for(i = 0; i < P7; i++)
	{
		free3a[i].is = false;
		free2a[i].is = false;
		free1a[i].is = false;
		d4d47[i].is = false;
		d3b7[i].is = false;
		d4b7[i].is = false;
	}
	for(i = 0; i < P8; i++)
	{
		d4d48[i].is = false;
		d3b8[i].is = false;
	}
	for(i = 0; i < P9; i++)
	{
		d4d49[i].is = false;
	}
	
	// lon
	for(i = 0; i < 2; i++)
	{
		lon[pat_tab[i][1]].is = true;
		lon[pat_tab[i][1]].macro = pat_tab[i][3];
		lon[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// five
	for(i = 2; i < 4; i++)
	{
		five[pat_tab[i][1]].is = true;
		five[pat_tab[i][1]].macro = pat_tab[i][3];
		five[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free4
	for(i = 4; i < 6; i++)
	{
		free4[pat_tab[i][1]].is = true;
		free4[pat_tab[i][1]].macro = pat_tab[i][3];
		free4[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead4
	for(i = 6; i < 16; i++)
	{
		dead4[pat_tab[i][1]].is = true;
		dead4[pat_tab[i][1]].macro = pat_tab[i][3];
		dead4[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free3
	for(i = 16; i < 24; i++)
	{
		free3[pat_tab[i][1]].is = true;
		free3[pat_tab[i][1]].macro = pat_tab[i][3];
		free3[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead3
	for(i = 24; i < 44; i++)
	{
		dead3[pat_tab[i][1]].is = true;
		dead3[pat_tab[i][1]].macro = pat_tab[i][3];
		dead3[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free2
	for(i = 44; i < 56; i++)
	{
		free2[pat_tab[i][1]].is = true;
		free2[pat_tab[i][1]].macro = pat_tab[i][3];
		free2[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead2
	for(i = 56; i < 76; i++)
	{
		dead2[pat_tab[i][1]].is = true;
		dead2[pat_tab[i][1]].macro = pat_tab[i][3];
		dead2[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free1
	for(i = 76; i < 84; i++)
	{
		free1[pat_tab[i][1]].is = true;
		free1[pat_tab[i][1]].macro = pat_tab[i][3];
		free1[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead1
	for(i = 84; i < 94; i++)
	{
		dead1[pat_tab[i][1]].is = true;
		dead1[pat_tab[i][1]].macro = pat_tab[i][3];
		dead1[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free3a
	for(i = 94; i < 96; i++)
	{
		free3a[pat_tab[i][1]].is = true;
		free3a[pat_tab[i][1]].macro = pat_tab[i][3];
		free3a[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free2a
	for(i = 96; i < 100; i++)
	{
		free2a[pat_tab[i][1]].is = true;
		free2a[pat_tab[i][1]].macro = pat_tab[i][3];
		free2a[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free1a
	for(i = 100; i < 102; i++)
	{
		free1a[pat_tab[i][1]].is = true;
		free1a[pat_tab[i][1]].macro = pat_tab[i][3];
		free1a[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d4d47
	for(i = 102; i < 104; i++)
	{
		d4d47[pat_tab[i][1]].is = true;
		d4d47[pat_tab[i][1]].macro = pat_tab[i][3];
		d4d47[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d4d48
	for(i = 104; i < 106; i++)
	{
		d4d48[pat_tab[i][1]].is = true;
		d4d48[pat_tab[i][1]].macro = pat_tab[i][3];
		d4d48[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d4d49
	for(i = 106; i < 108; i++)
	{
		d4d49[pat_tab[i][1]].is = true;
		d4d49[pat_tab[i][1]].macro = pat_tab[i][3];
		d4d49[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d4b7
	for(i = 108; i < 110; i++)
	{
		d4b7[pat_tab[i][1]].is = true;
		d4b7[pat_tab[i][1]].macro = pat_tab[i][3];
		d4b7[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d3b8
	for(i = 110; i < 114; i++)
	{
		d3b8[pat_tab[i][1]].is = true;
		d3b8[pat_tab[i][1]].macro = pat_tab[i][3];
		d3b8[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d3b7
	for(i = 114; i < 118; i++)
	{
		d3b7[pat_tab[i][1]].is = true;
		d3b7[pat_tab[i][1]].macro = pat_tab[i][3];
		d3b7[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// b6
	for(i = 118; i < 122; i++)
	{
		b6[pat_tab[i][1]].is = true;
		b6[pat_tab[i][1]].macro = pat_tab[i][3];
		b6[pat_tab[i][1]].mask = pat_tab[i][4];
	}
}

/*
 * Calculate pattern for a given line;
 *
 * @param[out]	tab			Pointer to one dimension of the table.
 * @param[in]	arr			Line array to be calculated.
 * @param[in]	len			Line length.
 */
static void line_pattern_cnt(u8* tab, const u8* arr, const int len)
{
	u16 mask = 0;
	int fiv[11], six[10], sev[9];
#if SLOW_TABLE
	int eig[8], nin[7];
#endif
	int bound5 = len - 4;
	int bound6 = len - 5;
	int bound7 = len - 6;
#if SLOW_TABLE
	int bound8 = len - 7;
	int bound9 = len - 8;
#endif
	int i, ind = 0;

	for(i = 0; i < PATTAB_NUM; i++)
		tab[i] = INVALID;

	for(i = 0; i < bound5; i++)
	{
		fiv[i] = arr[i] * P0 + arr[i + 1] * P1 + arr[i + 2] * P2
				+ arr[i + 3] * P3 + arr[i + 4] * P4;
	}
	for(i = 0; i < bound6; i++)
	{
		six[i] = arr[i] * P0 + arr[i + 1] * P1 + arr[i + 2] * P2
			+ arr[i + 3] * P3 + arr[i + 4] * P4 + arr[i + 5] * P5;
	}
	for(i = 0; i < bound7; i++)
	{
		sev[i] = arr[i] * P0 + arr[i + 1] * P1 + arr[i + 2] * P2
			+ arr[i + 3] * P3 + arr[i + 4] * P4 + arr[i + 5] * P5 
			+ arr[i + 6] * P6;
	}
#if SLOW_TABLE
	for(i = 0; i < bound8; i++)
	{
		eig[i] = arr[i] * P0 + arr[i + 1] * P1 + arr[i + 2] * P2
			+ arr[i + 3] * P3 + arr[i + 4] * P4 + arr[i + 5] * P5
			+ arr[i + 6] * P6 + arr[i + 7] * P7;
	}
	for(i = 0; i < bound9; i++)
	{
		nin[i] = arr[i] * P0 + arr[i + 1] * P1 + arr[i + 2] * P2
			+ arr[i + 3] * P3 + arr[i + 4] * P4 + arr[i + 5] * P5
			+ arr[i + 6] * P6 + arr[i + 7] * P7 + arr[i + 8] * P8;
	}
#endif

	// long
	if(isForbidden)
	{
		for(i = 0; i < bound6; i++)
		{
			if(lon[six[i]].is && !((mask >> i) & 0x3f))
			{
				tab[ind++] = lon[six[i]].macro;
#if SLOW_TABLE
				if(ind >= PATTAB_NUM) break;
				mask |= lon[six[i]].mask << i;
#else
				return;
#endif
			}
		}
	}
	// five
	for(i = 0; i < bound5; i++)
	{
		if(five[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			tab[ind++] = five[fiv[i]].macro;
#if SLOW_TABLE
			if(ind >= PATTAB_NUM) break;
			mask |= five[fiv[i]].mask << i;
#else
			return;
#endif
		}
	}
	if(isForbidden)
	{
#if SLOW_TABLE
		// d3b8
		for(i = 0; i < bound8; i++)
		{
			if(d3b8[eig[i]].is && !((mask >> i) & 0xff))
			{
				tab[ind++] = d3b8[eig[i]].macro;
				if(ind >= PATTAB_NUM) break;
				mask |= d3b8[eig[i]].mask << i;
			}
		}
#endif
		// d4b7
		for(i = 0; i < bound7; i++)
		{
			if(d4b7[sev[i]].is && !((mask >> i) & 0x7f))
			{
				tab[ind++] = d4b7[sev[i]].macro;
				if(ind >= PATTAB_NUM) break;
				mask |= d4b7[sev[i]].mask << i;
			}
		}
		// b6
		for(i = 0; i < bound6; i++)
		{
			if(b6[six[i]].is && !((mask >> i) & 0x3f))
				mask |= b6[six[i]].mask << i;
		}
	}
	// free4
	for(i = 0; i < bound6; i++)
	{
		if(free4[six[i]].is && !((mask >> i) & 0x3f))
		{
			tab[ind++] = free4[six[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free4[six[i]].mask << i;
		}
	}
#if SLOW_TABLE
	// d4d49
	for(i = 0; i < bound9; i++)
	{
		if(d4d49[nin[i]].is && !((mask >> i) & 0x1ff))
		{
			tab[ind++] = d4d49[nin[i]].macro;
			if(ind >= PATTAB_NUM) break;
			tab[ind++] = d4d49[nin[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= d4d49[nin[i]].mask << i;
		}
	}
	// d4d48
	for(i = 0; i < bound8; i++)
	{
		if(d4d48[eig[i]].is && !((mask >> i) & 0xff))
		{
			tab[ind++] = d4d48[eig[i]].macro;
			if(ind >= PATTAB_NUM) break;
			tab[ind++] = d4d48[eig[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= d4d48[eig[i]].mask << i;
		}
	}
#endif
	// d4d47
	for(i = 0; i < bound7; i++)
	{
		if(d4d47[sev[i]].is && !((mask >> i) & 0x7f))
		{
			tab[ind++] = d4d47[sev[i]].macro;
			if(ind >= PATTAB_NUM) break;
			tab[ind++] = d4d47[sev[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= d4d47[sev[i]].mask << i;
		}
	}
	// dead4
	for(i = 0; i < bound5; i++)
	{
		if(dead4[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			tab[ind++] = dead4[fiv[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= dead4[fiv[i]].mask << i;
		}
	}
#if SLOW_TABLE
	// d3b7
	if(isForbidden)
	{
		for(i = 0; i < bound7; i++)
		{
			if(d3b7[sev[i]].is && !((mask >> i) & 0x7f))
			{
				tab[ind++] = d3b7[sev[i]].macro;
				if(ind >= PATTAB_NUM) break;
				mask |= d3b7[sev[i]].mask << i;
			}
		}
	}
#endif
	// free3a
	for(i = 0; i < bound7; i++)
	{
		if(free3a[sev[i]].is && !((mask >> i) & 0x7f))
		{
			tab[ind++] = free3a[sev[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free3a[sev[i]].mask << i;
		}
	}
	// free3
	for(i = 0; i < bound6; i++)
	{
		if(free3[six[i]].is && !((mask >> i) & 0x3f))
		{
			tab[ind++] = free3[six[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free3[six[i]].mask << i;
		}
	}
	// dead3
	for(i = 0; i < bound5; i++)
	{
		if(dead3[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			tab[ind++] = dead3[fiv[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= dead3[fiv[i]].mask << i;
		}
	}
	// free2a
	for(i = 0; i < bound7; i++)
	{
		if(free2a[sev[i]].is && !((mask >> i) & 0x7f))
		{
			tab[ind++] = free2a[sev[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free2a[sev[i]].mask << i;
		}
	}
	// free2
	for(i = 0; i < bound6; i++)
	{
		if(free2[six[i]].is && !((mask >> i) & 0x3f))
		{
			tab[ind++] = free2[six[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free2[six[i]].mask << i;
		}
	}
	// dead2
	for(i = 0; i < bound5; i++)
	{
		if(dead2[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			tab[ind++] = dead2[fiv[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= dead2[fiv[i]].mask << i;
		}
	}
	// free1a
	for(i = 0; i < bound7; i++)
	{
		if(free1a[sev[i]].is && !((mask >> i) & 0x7f))
		{
			tab[ind++] = free1a[sev[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free1a[sev[i]].mask << i;
		}
	}
	// free1
	for(i = 0; i < bound6; i++)
	{
		if(free1[six[i]].is && !((mask >> i) & 0x3f))
		{
			tab[ind++] = free1[six[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= free1[six[i]].mask << i;
		}
	}
	// dead1
	for(i = 0; i < bound5; i++)
	{
		if(dead1[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			tab[ind++] = dead1[fiv[i]].macro;
			if(ind >= PATTAB_NUM) break;
			mask |= dead1[fiv[i]].mask << i;
		}
	}
}

void line_pattern_lookup(pattern_t* pat, const u32 index, const u8 len, const u8 op)
{
	u8* tab;
	int i;

	switch(len)
	{
		case 5:
			tab = pattab5[index];
			break;	
		case 6:
			tab = pattab6[index];
			break;	
		case 7:
			tab = pattab7[index];
			break;	
		case 8:
			tab = pattab8[index];
			break;	
		case 9:
			tab = pattab9[index];
			break;	
		case 10:
			tab = pattab10[index];
			break;	
		case 11:
			tab = pattab11[index];
			break;	
		case 12:
			tab = pattab12[index];
			break;	
		case 13:
			tab = pattab13[index];
			break;	
		case 14:
			tab = pattab14[index];
			break;	
		case 15:
			tab = pattab15[index];
			break;	
		default:
			return;
	}

	for(i = 0; i < PATTAB_NUM; i++)
	{
		if(tab[i] != INVALID)
		{
			if(op == INC)
				pattern_inc(pat, pattab_trans[tab[i]][0], pattab_trans[tab[i]][1]);
			else if(op == DEC)
				pattern_dec(pat, pattab_trans[tab[i]][0], pattab_trans[tab[i]][1]);
			else
				break;
		}
		else
			break;
	}
}

/*******************************************************************************
						Pattern lookup table generation
*******************************************************************************/
static void pattab15_init()
{
	u32 i;
	u8 a[15];
	for(a[14] = EMPTY; a[14] <= WHITE; a[14]++) {
	for(a[13] = EMPTY; a[13] <= WHITE; a[13]++) {
	for(a[12] = EMPTY; a[12] <= WHITE; a[12]++) {
	for(a[11] = EMPTY; a[11] <= WHITE; a[11]++) {
	for(a[10] = EMPTY; a[10] <= WHITE; a[10]++) {
	for(a[9] = EMPTY; a[9] <= WHITE; a[9]++) {
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8 + a[9]*P9 + a[10]*P10 + a[11]*P11 + a[12]*P12
		+ a[13]*P13 + a[14]*P14;
	line_pattern_cnt(pattab15[i], a, 15);
	} } } } } } } } } } } } } } }
}

static void pattab14_init()
{
	u32 i;
	u8 a[14];
	for(a[13] = EMPTY; a[13] <= WHITE; a[13]++) {
	for(a[12] = EMPTY; a[12] <= WHITE; a[12]++) {
	for(a[11] = EMPTY; a[11] <= WHITE; a[11]++) {
	for(a[10] = EMPTY; a[10] <= WHITE; a[10]++) {
	for(a[9] = EMPTY; a[9] <= WHITE; a[9]++) {
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8 + a[9]*P9 + a[10]*P10 + a[11]*P11 + a[12]*P12
		+ a[13]*P13;
	line_pattern_cnt(pattab14[i], a, 14);
	} } } } } } } } } } } } } }
}

static void pattab13_init()
{
	u32 i;
	u8 a[13];
	for(a[12] = EMPTY; a[12] <= WHITE; a[12]++) {
	for(a[11] = EMPTY; a[11] <= WHITE; a[11]++) {
	for(a[10] = EMPTY; a[10] <= WHITE; a[10]++) {
	for(a[9] = EMPTY; a[9] <= WHITE; a[9]++) {
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8 + a[9]*P9 + a[10]*P10 + a[11]*P11 + a[12]*P12;
	line_pattern_cnt(pattab13[i], a, 13);
	} } } } } } } } } } } } }
}

static void pattab12_init()
{
	u32 i;
	u8 a[12];
	for(a[11] = EMPTY; a[11] <= WHITE; a[11]++) {
	for(a[10] = EMPTY; a[10] <= WHITE; a[10]++) {
	for(a[9] = EMPTY; a[9] <= WHITE; a[9]++) {
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8 + a[9]*P9 + a[10]*P10 + a[11]*P11;
	line_pattern_cnt(pattab12[i], a, 12);
	} } } } } } } } } } } }
}

static void pattab11_init()
{
	u32 i;
	u8 a[11];
	for(a[10] = EMPTY; a[10] <= WHITE; a[10]++) {
	for(a[9] = EMPTY; a[9] <= WHITE; a[9]++) {
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8 + a[9]*P9 + a[10]*P10;
	line_pattern_cnt(pattab11[i], a, 11);
	} } } } } } } } } } }
}

static void pattab10_init()
{
	u32 i;
	u8 a[10];
	for(a[9] = EMPTY; a[9] <= WHITE; a[9]++) {
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8 + a[9]*P9;
	line_pattern_cnt(pattab10[i], a, 10);
	} } } } } } } } } }
}

static void pattab9_init()
{
	u32 i;
	u8 a[9];
	for(a[8] = EMPTY; a[8] <= WHITE; a[8]++) {
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7 + a[8]*P8;
	line_pattern_cnt(pattab9[i], a, 9);
	} } } } } } } } }
}

static void pattab8_init()
{
	u32 i;
	u8 a[8];
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6
		+ a[7]*P7;
	line_pattern_cnt(pattab8[i], a, 8);
	} } } } } } } }
}

static void pattab7_init()
{
	u32 i;
	u8 a[7];
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5 + a[6]*P6;
	line_pattern_cnt(pattab7[i], a, 7);
	} } } } } } }
}

static void pattab6_init()
{
	u32 i;
	u8 a[6];
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4 + a[5]*P5;
	line_pattern_cnt(pattab6[i], a, 6);
	} } } } } }
}

static void pattab5_init()
{
	u32 i;
	u8 a[5];
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {
	i = a[0]*P0 + a[1]*P1 + a[2]*P2 + a[3]*P3 + a[4]*P4;
	line_pattern_cnt(pattab5[i], a, 5);
	} } } } }
}

void pattern_table_init1()
{
	pat_t_init();
	pattab15_init();
}

void pattern_table_init2()
{
	pattab14_init();
	pattab13_init();
	pattab12_init();
	pattab11_init();
	pattab10_init();
	pattab9_init();
	pattab8_init();
	pattab7_init();
	pattab6_init();
	pattab5_init();
}

