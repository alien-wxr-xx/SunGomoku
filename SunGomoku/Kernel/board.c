/*                     _______
 *  Gomoku software   / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * board.c - implementation of board_t data structure
 */

#include "board.h"
#include "macro.h"
#include "pattern.h"
#include "mvlist.h"

// powers of three
#define P0		1
#define P1		3
#define P2		9
#define P3		27
#define P4		81
#define P5		243
#define P6		729
#define P7		2187
#define P8		6561
#define P9		19683
#define P10		59049
#define P11		177147
#define P12		531441
#define P13		1594323
#define P14		4782969
#define P15		14348907

// global variable controlling if consider forbidden points
bool isForbidden = true;

/*******************************************************************************
							Neighbor table generation
*******************************************************************************/
#define NEI_SIZE	16
#define NEI_DEBUG	0

static u8 nei[15 * 15][NEI_SIZE];
static u8 nei_helper[19][19];

#if NEI_DEBUG
static void print_nei_helper()
{
	int r, c;
	for(r = 0; r < 19; r++)
	{
		for(c = 0; c < 19; c++)
			printf("%d\t", nei_helper[r][c]);
		putchar('\n');
		putchar('\n');
	}
}

static void print_nei()
{
	int r, c, i;
	for(r = 0; r < 15; r++)
	{
		for(c = 0; c < 15; c++)
		{
			printf("(%d,%d):  \t", r, c);
			for(i = 0; i < NEI_SIZE; i++)
				if(nei[r * 15 + c][i] != INVALID)
					printf("%d  ", nei[r * 15 + c][i]);
			putchar('\n');
		}
	}
	putchar('\n');
}
#endif

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
#if NEI_DEBUG
	print_nei_helper();
#endif
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
#if NEI_DEBUG
	print_nei();
#endif
}

/*******************************************************************************
							Line count implementation
*******************************************************************************/
#define PAT_ROW		122

// helper structure
typedef struct {
	bool is;
	u8 color;
	u8 mask;
} pat_t;

// pat_t tables
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

// array for generating lookup tables
// one row represents pattern length, index, type, color and mask
// overlapping patterns are not considered
static u16 pat_tab[PAT_ROW][5] = 
{
	// 1-2
	{ 6, 364, LONG,  BLACK, 0x3f },		// 111111
	{ 6, 728, LONG,  WHITE, 0x3f },

	// 3-4
	{ 5, 121, FIVE,  BLACK, 0x1f },		// 11111
	{ 5, 242, FIVE,  WHITE, 0x1f },

	// 5-6
	{ 6, 120, FREE4, BLACK, 0x1e },		// 011110
	{ 6, 240, FREE4, WHITE, 0x1e },
	
	// 7-16
	{ 5, 120, DEAD4, BLACK, 0x1e },		// 11110
	{ 5, 240, DEAD4, WHITE, 0x1e },
	{ 5, 118, DEAD4, BLACK, 0x1d },		// 11101
	{ 5, 236, DEAD4, WHITE, 0x1d },
	{ 5, 112, DEAD4, BLACK, 0x1b },		// 11011
	{ 5, 224, DEAD4, WHITE, 0x1b },
	{ 5, 94,  DEAD4, BLACK, 0x17 },		// 10111
	{ 5, 188, DEAD4, WHITE, 0x17 },
	{ 5, 40,  DEAD4, BLACK, 0x0f },		// 01111
	{ 5, 80,  DEAD4, WHITE, 0x0f },

	// 17-24
	{ 6, 117, FREE3, BLACK, 0x1c },		// 011100
	{ 6, 234, FREE3, WHITE, 0x1c },
	{ 6, 111, FREE3, BLACK, 0x1a },		// 011010
	{ 6, 222, FREE3, WHITE, 0x1a },
	{ 6, 93,  FREE3, BLACK, 0x16 },		// 010110
	{ 6, 186, FREE3, WHITE, 0x16 },
	{ 6, 39,  FREE3, BLACK, 0x0e },		// 001110
	{ 6, 78,  FREE3, WHITE, 0x0e },

	// 25-44
	{ 5, 117, DEAD3, BLACK, 0x1c},		// 11100
	{ 5, 234, DEAD3, WHITE, 0x1c},
	{ 5, 111, DEAD3, BLACK, 0x1a},		// 11010
	{ 5, 222, DEAD3, WHITE, 0x1a},
	{ 5, 93,  DEAD3, BLACK, 0x16},		// 10110
	{ 5, 186, DEAD3, WHITE, 0x16},
	{ 5, 39,  DEAD3, BLACK, 0x0e},		// 01110
	{ 5, 78,  DEAD3, WHITE, 0x0e},
	{ 5, 109, DEAD3, BLACK, 0x19},		// 11001
	{ 5, 218, DEAD3, WHITE, 0x19},
	{ 5, 91,  DEAD3, BLACK, 0x15},		// 10101
	{ 5, 182, DEAD3, WHITE, 0x15},
	{ 5, 37,  DEAD3, BLACK, 0x0d},		// 01101
	{ 5, 74,  DEAD3, WHITE, 0x0d},
	{ 5, 85,  DEAD3, BLACK, 0x13},		// 10011
	{ 5, 170, DEAD3, WHITE, 0x13},
	{ 5, 31,  DEAD3, BLACK, 0x0b},		// 01011
	{ 5, 62,  DEAD3, WHITE, 0x0b},
	{ 5, 13,  DEAD3, BLACK, 0x07},		// 00111
	{ 5, 26,  DEAD3, WHITE, 0x07},

	// 45-56
	{ 6, 108, FREE2, BLACK, 0x18 },		// 011000
	{ 6, 216, FREE2, WHITE, 0x18 },
	{ 6, 90,  FREE2, BLACK, 0x14 },		// 010100
	{ 6, 180, FREE2, WHITE, 0x14 },
	{ 6, 36,  FREE2, BLACK, 0x0c },		// 001100
	{ 6, 72,  FREE2, WHITE, 0x0c },
	{ 6, 84,  FREE2, BLACK, 0x12 },		// 010010
	{ 6, 168, FREE2, WHITE, 0x12 },
	{ 6, 30,  FREE2, BLACK, 0x0a },		// 001010
	{ 6, 60,  FREE2, WHITE, 0x0a },
	{ 6, 12,  FREE2, BLACK, 0x06 },		// 000110
	{ 6, 24,  FREE2, WHITE, 0x06 },

	// 57-76
	{ 5, 4,   DEAD2, BLACK, 0x03 },		// 00011
	{ 5, 8,   DEAD2, WHITE, 0x03 },
	{ 5, 10,  DEAD2, BLACK, 0x05 },		// 00101
	{ 5, 20,  DEAD2, WHITE, 0x05 },
	{ 5, 28,  DEAD2, BLACK, 0x09 },		// 01001
	{ 5, 56,  DEAD2, WHITE, 0x09 },
	{ 5, 82,  DEAD2, BLACK, 0x11 },		// 10001
	{ 5, 164, DEAD2, WHITE, 0x11 },
	{ 5, 12,  DEAD2, BLACK, 0x06 },		// 00110
	{ 5, 24,  DEAD2, WHITE, 0x06 },
	{ 5, 30,  DEAD2, BLACK, 0x0a },		// 01010
	{ 5, 60,  DEAD2, WHITE, 0x0a },
	{ 5, 84,  DEAD2, BLACK, 0x12 },		// 10010
	{ 5, 168, DEAD2, WHITE, 0x12 },
	{ 5, 36,  DEAD2, BLACK, 0x0c },		// 01100
	{ 5, 72,  DEAD2, WHITE, 0x0c },
	{ 5, 90,  DEAD2, BLACK, 0x14 },		// 10100
	{ 5, 180, DEAD2, WHITE, 0x14 },
	{ 5, 108, DEAD2, BLACK, 0x18 },		// 11000
	{ 5, 216, DEAD2, WHITE, 0x18 },

	// 77-84
	{ 6, 3,   FREE1, BLACK, 0x02 },		// 000010
	{ 6, 6,   FREE1, WHITE, 0x02 },
	{ 6, 9,   FREE1, BLACK, 0x04 },		// 000100
	{ 6, 18,  FREE1, WHITE, 0x04 },
	{ 6, 27,  FREE1, BLACK, 0x08 },		// 001000
	{ 6, 54,  FREE1, WHITE, 0x08 },
	{ 6, 81,  FREE1, BLACK, 0x10 },		// 010000
	{ 6, 162, FREE1, WHITE, 0x10 },

	// 85-94
	{ 5, 1,   DEAD1, BLACK, 0x01 },		// 00001
	{ 5, 2,   DEAD1, WHITE, 0x01 },
	{ 5, 3,   DEAD1, BLACK, 0x02 },		// 00010
	{ 5, 6,   DEAD1, WHITE, 0x02 },
	{ 5, 9,   DEAD1, BLACK, 0x04 },		// 00100
	{ 5, 18,  DEAD1, WHITE, 0x04 },
	{ 5, 27,  DEAD1, BLACK, 0x08 },		// 01000
	{ 5, 54,  DEAD1, WHITE, 0x08 },
	{ 5, 81,  DEAD1, BLACK, 0x10 },		// 10000
	{ 5, 162, DEAD1, WHITE, 0x10 },

	// 95-96
	{ 7, 117, FREE3a, BLACK, 0x1c },	// 0011100
	{ 7, 234, FREE3a, WHITE, 0x1c },

	// 97-100
	{ 7, 108, FREE2a, BLACK, 0x18 },	// 0011000
	{ 7, 216, FREE2a, WHITE, 0x18 },
	{ 7, 36,  FREE2a, BLACK, 0x0c },	// 0001100
	{ 7, 72,  FREE2a, WHITE, 0x0c },

	// 101-102
	{ 7, 27,  FREE1a, BLACK, 0x08 },	// 0001000
	{ 7, 54,  FREE1a, WHITE, 0x08 },

	// 103-104
	{ 7, 847, DEAD4, BLACK, 0x5d },		// 1011101
	{ 7, 1694,DEAD4, WHITE, 0x5d },

	// 105-106
	{ 8, 3028,DEAD4, BLACK, 0xdb },		// 11011011
	{ 8, 6056,DEAD4, WHITE, 0xdb },

	// 107-108
	{ 9, 9571,DEAD4, BLACK, 0x1d7},		// 111010111
	{ 9,19142,DEAD4, WHITE, 0x1d7},

	// 109-110
	{ 7, 361, DEAD4, BLACK, 0x3c },		// 0111101
	{ 7, 849, DEAD4, BLACK, 0x1e },		// 1011110

	// 111-114
	{ 8, 355, DEAD3, BLACK, 0x3b },		// 00111011
	{ 8, 3033,DEAD3, BLACK, 0xdc },		// 11011100
	{ 8, 4726,DEAD3, BLACK, 0x38 },		// 20111001
	{ 8, 2306,DEAD3, BLACK, 0x1c },		// 10011102

	// 115-118
	{ 7, 822, DEAD3, BLACK, 0x16 },		// 1010110
	{ 7, 840, DEAD3, BLACK, 0x1a },		// 1011010
	{ 7, 280, DEAD3, BLACK, 0x2c },		// 0101101
	{ 7, 334, DEAD3, BLACK, 0x34 },		// 0110101

	// 119-122
	{ 6, 361,    99, BLACK, 0x3c },		// 111101
	{ 6, 355,    99, BLACK, 0x3b },		// 111011
	{ 6, 337,    99, BLACK, 0x27 },		// 110111
	{ 6, 283,    99, BLACK, 0x1e }		// 101111
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
		lon[pat_tab[i][1]].color = pat_tab[i][3];
		lon[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// five
	for(i = 2; i < 4; i++)
	{
		five[pat_tab[i][1]].is = true;
		five[pat_tab[i][1]].color = pat_tab[i][3];
		five[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free4
	for(i = 4; i < 6; i++)
	{
		free4[pat_tab[i][1]].is = true;
		free4[pat_tab[i][1]].color = pat_tab[i][3];
		free4[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead4
	for(i = 6; i < 16; i++)
	{
		dead4[pat_tab[i][1]].is = true;
		dead4[pat_tab[i][1]].color = pat_tab[i][3];
		dead4[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free3
	for(i = 16; i < 24; i++)
	{
		free3[pat_tab[i][1]].is = true;
		free3[pat_tab[i][1]].color = pat_tab[i][3];
		free3[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead3
	for(i = 24; i < 44; i++)
	{
		dead3[pat_tab[i][1]].is = true;
		dead3[pat_tab[i][1]].color = pat_tab[i][3];
		dead3[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free2
	for(i = 44; i < 56; i++)
	{
		free2[pat_tab[i][1]].is = true;
		free2[pat_tab[i][1]].color = pat_tab[i][3];
		free2[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead2
	for(i = 56; i < 76; i++)
	{
		dead2[pat_tab[i][1]].is = true;
		dead2[pat_tab[i][1]].color = pat_tab[i][3];
		dead2[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free1
	for(i = 76; i < 84; i++)
	{
		free1[pat_tab[i][1]].is = true;
		free1[pat_tab[i][1]].color = pat_tab[i][3];
		free1[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// dead1
	for(i = 84; i < 94; i++)
	{
		dead1[pat_tab[i][1]].is = true;
		dead1[pat_tab[i][1]].color = pat_tab[i][3];
		dead1[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free3a
	for(i = 94; i < 96; i++)
	{
		free3a[pat_tab[i][1]].is = true;
		free3a[pat_tab[i][1]].color = pat_tab[i][3];
		free3a[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free2a
	for(i = 96; i < 100; i++)
	{
		free2a[pat_tab[i][1]].is = true;
		free2a[pat_tab[i][1]].color = pat_tab[i][3];
		free2a[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// free1a
	for(i = 100; i < 102; i++)
	{
		free1a[pat_tab[i][1]].is = true;
		free1a[pat_tab[i][1]].color = pat_tab[i][3];
		free1a[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	//d4d47
	for(i = 102; i < 104; i++)
	{
		d4d47[pat_tab[i][1]].is = true;
		d4d47[pat_tab[i][1]].color = pat_tab[i][3];
		d4d47[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	//d4d48
	for(i = 104; i < 106; i++)
	{
		d4d48[pat_tab[i][1]].is = true;
		d4d48[pat_tab[i][1]].color = pat_tab[i][3];
		d4d48[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	//d4d49
	for(i = 106; i < 108; i++)
	{
		d4d49[pat_tab[i][1]].is = true;
		d4d49[pat_tab[i][1]].color = pat_tab[i][3];
		d4d49[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d4b7
	for(i = 108; i < 110; i++)
	{
		d4b7[pat_tab[i][1]].is = true;
		d4b7[pat_tab[i][1]].color = pat_tab[i][3];
		d4b7[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d3b8
	for(i = 110; i < 114; i++)
	{
		d3b8[pat_tab[i][1]].is = true;
		d3b8[pat_tab[i][1]].color = pat_tab[i][3];
		d3b8[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// d3b7
	for(i = 114; i < 118; i++)
	{
		d3b7[pat_tab[i][1]].is = true;
		d3b7[pat_tab[i][1]].color = pat_tab[i][3];
		d3b7[pat_tab[i][1]].mask = pat_tab[i][4];
	}
	// b6
	for(i = 118; i < 122; i++)
	{
		b6[pat_tab[i][1]].is = true;
		b6[pat_tab[i][1]].color = pat_tab[i][3];
		b6[pat_tab[i][1]].mask = pat_tab[i][4];
	}
}

// count patterns of a given line array
// len is array length
static inline void line_cnt(pattern_t* pat, const u8* arr, const int len)
{
	u16 mask = 0;
	int fiv[11], six[10], sev[9], eig[8], nin[7];
	int bound5 = len - 4;
	int bound6 = len - 5;
	int bound7 = len - 6;
	int bound8 = len - 7;
	int bound9 = len - 8;
	int i;

	pattern_reset(pat);

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

	// long
	if(isForbidden)
	{
		for(i = 0; i < bound6; i++)
		{
			if(lon[six[i]].is && !((mask >> i) & 0x3f))
			{
				pattern_inc(pat, LONG, lon[six[i]].color);
				mask |= lon[six[i]].mask << i;
			}
		}
	}
	// five
	for(i = 0; i < bound5; i++)
	{
		if(five[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			pattern_inc(pat, FIVE, five[fiv[i]].color);
			mask |= five[fiv[i]].mask << i;
		}
	}
	// d3b8
	if(isForbidden)
	{
		for(i = 0; i < bound8; i++)
		{
			if(d3b8[eig[i]].is && !((mask >> i) & 0xff))
			{
				pattern_inc(pat, DEAD3, d3b8[eig[i]].color);
				mask |= d3b8[eig[i]].mask << i;
			}
		}
		// d4b7
		for(i = 0; i < bound7; i++)
		{
			if(d4b7[sev[i]].is && !((mask >> i) & 0x7f))
			{
				pattern_inc(pat, DEAD4, d4b7[sev[i]].color);
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
			pattern_inc(pat, FREE4, free4[six[i]].color);
			mask |= free4[six[i]].mask << i;
		}
	}
	// d4d49
	for(i = 0; i < bound9; i++)
	{
		if(d4d49[nin[i]].is && !((mask >> i) & 0x1ff))
		{
			pattern_inc(pat, DEAD4, d4d49[nin[i]].color);
			pattern_inc(pat, DEAD4, d4d49[nin[i]].color);
			mask |= d4d49[nin[i]].mask << i;
		}
	}
	// d4d48
	for(i = 0; i < bound8; i++)
	{
		if(d4d48[eig[i]].is && !((mask >> i) & 0xff))
		{
			pattern_inc(pat, DEAD4, d4d48[eig[i]].color);
			pattern_inc(pat, DEAD4, d4d48[eig[i]].color);
			mask |= d4d48[eig[i]].mask << i;
		}
	}
	// d4d47
	for(i = 0; i < bound7; i++)
	{
		if(d4d47[sev[i]].is && !((mask >> i) & 0x7f))
		{
			pattern_inc(pat, DEAD4, d4d47[sev[i]].color);
			pattern_inc(pat, DEAD4, d4d47[sev[i]].color);
			mask |= d4d47[sev[i]].mask << i;
		}
	}
	// dead4
	for(i = 0; i < bound5; i++)
	{
		if(dead4[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			pattern_inc(pat, DEAD4, dead4[fiv[i]].color);
			mask |= dead4[fiv[i]].mask << i;
		}
	}
	// d3b7
	if(isForbidden)
	{
		for(i = 0; i < bound7; i++)
		{
			if(d3b7[sev[i]].is && !((mask >> i) & 0x7f))
			{
				pattern_inc(pat, DEAD3, d3b7[sev[i]].color);
				mask |= d3b7[sev[i]].mask << i;
			}
		}
	}
	// free3a
	for(i = 0; i < bound7; i++)
	{
		if(free3a[sev[i]].is && !((mask >> i) & 0x7f))
		{
			pattern_inc(pat, FREE3a, free3a[sev[i]].color);
			mask |= free3a[sev[i]].mask << i;
		}
	}
	// free3
	for(i = 0; i < bound6; i++)
	{
		if(free3[six[i]].is && !((mask >> i) & 0x3f))
		{
			pattern_inc(pat, FREE3, free3[six[i]].color);
			mask |= free3[six[i]].mask << i;
		}
	}
	// dead3
	for(i = 0; i < bound5; i++)
	{
		if(dead3[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			pattern_inc(pat, DEAD3, dead3[fiv[i]].color);
			mask |= dead3[fiv[i]].mask << i;
		}
	}
	// free2a
	for(i = 0; i < bound7; i++)
	{
		if(free2a[sev[i]].is && !((mask >> i) & 0x7f))
		{
			pattern_inc(pat, FREE2a, free2a[sev[i]].color);
			mask |= free2a[sev[i]].mask << i;
		}
	}
	// free2
	for(i = 0; i < bound6; i++)
	{
		if(free2[six[i]].is && !((mask >> i) & 0x3f))
		{
			pattern_inc(pat, FREE2, free2[six[i]].color);
			mask |= free2[six[i]].mask << i;
		}
	}
	// dead2
	for(i = 0; i < bound5; i++)
	{
		if(dead2[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			pattern_inc(pat, DEAD2, dead2[fiv[i]].color);
			mask |= dead2[fiv[i]].mask << i;
		}
	}
	// free1a
	for(i = 0; i < bound7; i++)
	{
		if(free1a[sev[i]].is && !((mask >> i) & 0x7f))
		{
			pattern_inc(pat, FREE1a, free1a[sev[i]].color);
			mask |= free1a[sev[i]].mask << i;
		}
	}
	// free1
	for(i = 0; i < bound6; i++)
	{
		if(free1[six[i]].is && !((mask >> i) & 0x3f))
		{
			pattern_inc(pat, FREE1, free1[six[i]].color);
			mask |= free1[six[i]].mask << i;
		}
	}
	// dead1
	for(i = 0; i < bound5; i++)
	{
		if(dead1[fiv[i]].is && !((mask >> i) & 0x1f))
		{
			pattern_inc(pat, DEAD1, dead1[fiv[i]].color);
			mask |= dead1[fiv[i]].mask << i;
		}
	}
}

/*******************************************************************************
						Pattern lookup table generation
*******************************************************************************/
// pattern lookup tables
static pattern_t table15[P15];
static pattern_t table14[P14];
static pattern_t table13[P13];
static pattern_t table12[P12];
static pattern_t table11[P11];
static pattern_t table10[P10];
static pattern_t table9[P9];
static pattern_t table8[P8];
static pattern_t table7[P7];
static pattern_t table6[P6];
static pattern_t table5[P5];

// generate pattern lookup tables
static void table15_init()
{
	u32 index;
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
	index = a[14]*P14 + a[13]*P13 + a[12]*P12 + a[11]*P11 + a[10]*P10
			+ a[9]*P9 + a[8]*P8 + a[7]*P7 + a[6]*P6 + a[5]*P5 + a[4]*P4
			+ a[3]*P3 + a[2]*P2 + a[1]*P1 + a[0]*P0;
	line_cnt(&table15[index], a, 15);
	} } } } } } } } } } } } } } }
}

static void table14_init()
{
	u32 index;
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
	index = a[13]*P13 + a[12]*P12 + a[11]*P11 + a[10]*P10 + a[9]*P9 + a[8]*P8
			+ a[7]*P7 + a[6]*P6 + a[5]*P5 + a[4]*P4 + a[3]*P3 + a[2]*P2
			+ a[1]*P1 + a[0]*P0;
	line_cnt(&table14[index], a, 14);
	} } } } } } } } } } } } } }
}

static void table13_init()
{
	u32 index;
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
	index = a[12]*P12 + a[11]*P11 + a[10]*P10 + a[9]*P9 + a[8]*P8 + a[7]*P7
			+ a[6]*P6 + a[5]*P5 + a[4]*P4 + a[3]*P3 + a[2]*P2 + a[1]*P1
			+ a[0]*P0;
	line_cnt(&table13[index], a, 13);
	} } } } } } } } } } } } }
}

static void table12_init()
{
	u32 index;
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
	index = a[11]*P11 + a[10]*P10 + a[9]*P9 + a[8]*P8 + a[7]*P7 + a[6]*P6
			+ a[5]*P5 + a[4]*P4 + a[3]*P3 + a[2]*P2 + a[1]*P1 + a[0]*P0;
	line_cnt(&table12[index], a, 12);
	} } } } } } } } } } } }
}

static void table11_init()
{
	u32 index;
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
	index = a[10]*P10 + a[9]*P9 + a[8]*P8 + a[7]*P7 + a[6]*P6 + a[5]*P5
			+ a[4]*P4 + a[3]*P3 + a[2]*P2 + a[1]*P1 + a[0]*P0;
	line_cnt(&table11[index], a, 11);
	} } } } } } } } } } }
}

static void table10_init()
{
	u32 index;
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
	index = a[9]*P9 + a[8]*P8 + a[7]*P7	+ a[6]*P6 + a[5]*P5 + a[4]*P4
			+ a[3]*P3 + a[2]*P2	+ a[1]*P1 + a[0]*P0;
	line_cnt(&table10[index], a, 10);
	} } } } } } } } } }
}

static void table9_init()
{
	u32 index;
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
	index = a[8]*P8 + a[7]*P7 + a[6]*P6 + a[5]*P5 + a[4]*P4 + a[3]*P3
			+ a[2]*P2 + a[1]*P1 + a[0]*P0;
	line_cnt(&table9[index], a, 9);
	} } } } } } } } }
}

static void table8_init()
{
	u32 index;
	u8 a[8];
	for(a[7] = EMPTY; a[7] <= WHITE; a[7]++) {
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {	
	index = a[7]*P7 + a[6]*P6 + a[5]*P5 + a[4]*P4 + a[3]*P3 + a[2]*P2
			+ a[1]*P1 + a[0]*P0;
	line_cnt(&table8[index], a, 8);
	} } } } } } } }
}

static void table7_init()
{
	u32 index;
	u8 a[7];
	for(a[6] = EMPTY; a[6] <= WHITE; a[6]++) {
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {	
	index = a[6]*P6 + a[5]*P5 + a[4]*P4 + a[3]*P3 + a[2]*P2	+ a[1]*P1
			+ a[0]*P0;
	line_cnt(&table7[index], a, 7);
	} } } } } } }
}

static void table6_init()
{
	u32 index;
	u8 a[6];
	for(a[5] = EMPTY; a[5] <= WHITE; a[5]++) {
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {	
	index = a[5]*P5 + a[4]*P4 + a[3]*P3 + a[2]*P2 + a[1]*P1 + a[0]*P0;
	line_cnt(&table6[index], a, 6);
	} } } } } }
}

static void table5_init()
{
	u32 index;
	u8 a[5];
	for(a[4] = EMPTY; a[4] <= WHITE; a[4]++) {
	for(a[3] = EMPTY; a[3] <= WHITE; a[3]++) {
	for(a[2] = EMPTY; a[2] <= WHITE; a[2]++) {
	for(a[1] = EMPTY; a[1] <= WHITE; a[1]++) {
	for(a[0] = EMPTY; a[0] <= WHITE; a[0]++) {	
	index = a[4]*P4 + a[3]*P3 + a[2]*P2 + a[1]*P1 + a[0]*P0;
	line_cnt(&table5[index], a, 5);
	} } } } }
}

void pattern_table_init1()
{
	pat_t_init();
	table15_init();
}

void pattern_table_init2()
{
	table14_init();
	table13_init();
	table12_init();
	table11_init();
	table10_init();
	table9_init();
	table8_init();
	table7_init();
	table6_init();
	table5_init();
}

/*******************************************************************************
							Functions calculating index
*******************************************************************************/
// macros calculating table index
#define IND15(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)	( \
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11 +	\
	bd->arr[a13]*P12 + bd->arr[a14]*P13 + bd->arr[a15]*P14						\
)

#define IND14(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)	( 	\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11 +	\
	bd->arr[a13]*P12 + bd->arr[a14]*P13											\
)

#define IND13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)	( 		\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11 +	\
	bd->arr[a13]*P12															\
)

#define IND12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)	( 			\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11		\
)

#define IND11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)	( 					\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10							\
)

#define IND10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)	( 						\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9											\
)

#define IND9(a1, a2, a3, a4, a5, a6, a7, a8, a9)	( 							\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8																\
)

#define IND8(a1, a2, a3, a4, a5, a6, a7, a8)	( 								\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7			\
)

#define IND7(a1, a2, a3, a4, a5, a6, a7)	( 									\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6							\
)

#define IND6(a1, a2, a3, a4, a5, a6)	( 										\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5												\
)

#define IND5(a1, a2, a3, a4, a5)	( 											\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4																\
)

// return row indices
inline static u32 r1(board_t* bd)
{
	return IND15(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
}

inline static u32 r2(board_t* bd)
{
	return IND15(15,16,17,18,19,20,21,22,23,24,25,26,27,28,29);
}
inline static u32 r3(board_t* bd)
{
	return IND15(30,31,32,33,34,35,36,37,38,39,40,41,42,43,44);
}

inline static u32 r4(board_t* bd)
{
	return IND15(45,46,47,48,49,50,51,52,53,54,55,56,57,58,59);
}

inline static u32 r5(board_t* bd)
{
	return IND15(60,61,62,63,64,65,66,67,68,69,70,71,72,73,74);
}

inline static u32 r6(board_t* bd)
{
	return IND15(75,76,77,78,79,80,81,82,83,84,85,86,87,88,89);
}

inline static u32 r7(board_t* bd)
{
	return IND15(90,91,92,93,94,95,96,97,98,99,100,101,102,103,104);
}

inline static u32 r8(board_t* bd)
{
	return IND15(105,106,107,108,109,110,111,112,113,114,115,116,117,118,119);
}

inline static u32 r9(board_t* bd)
{
	return IND15(120,121,122,123,124,125,126,127,128,129,130,131,132,133,134);
}

inline static u32 r10(board_t* bd)
{
	return IND15(135,136,137,138,139,140,141,142,143,144,145,146,147,148,149);
}

inline static u32 r11(board_t* bd)
{
	return IND15(150,151,152,153,154,155,156,157,158,159,160,161,162,163,164);
}

inline static u32 r12(board_t* bd)
{
	return IND15(165,166,167,168,169,170,171,172,173,174,175,176,177,178,179);
}

inline static u32 r13(board_t* bd)
{
	return IND15(180,181,182,183,184,185,186,187,188,189,190,191,192,193,194);
}

inline static u32 r14(board_t* bd)
{
	return IND15(195,196,197,198,199,200,201,202,203,204,205,206,207,208,209);
}

inline static u32 r15(board_t* bd)
{
	return IND15(210,211,212,213,214,215,216,217,218,219,220,221,222,223,224);
}

// return column indices
inline static u32 c1(board_t* bd)
{
	return IND15(0,15,30,45,60,75,90,105,120,135,150,165,180,195,210);
}

inline static u32 c2(board_t* bd)
{
	return IND15(1,16,31,46,61,76,91,106,121,136,151,166,181,196,211);
}

inline static u32 c3(board_t* bd)
{
	return IND15(2,17,32,47,62,77,92,107,122,137,152,167,182,197,212);
}

inline static u32 c4(board_t* bd)
{
	return IND15(3,18,33,48,63,78,93,108,123,138,153,168,183,198,213);
}

inline static u32 c5(board_t* bd)
{
	return IND15(4,19,34,49,64,79,94,109,124,139,154,169,184,199,214);
}

inline static u32 c6(board_t* bd)
{
	return IND15(5,20,35,50,65,80,95,110,125,140,155,170,185,200,215);
}

inline static u32 c7(board_t* bd)
{
	return IND15(6,21,36,51,66,81,96,111,126,141,156,171,186,201,216);
}

inline static u32 c8(board_t* bd)
{
	return IND15(7,22,37,52,67,82,97,112,127,142,157,172,187,202,217);
}

inline static u32 c9(board_t* bd)
{
	return IND15(8,23,38,53,68,83,98,113,128,143,158,173,188,203,218);
}

inline static u32 c10(board_t* bd)
{
	return IND15(9,24,39,54,69,84,99,114,129,144,159,174,189,204,219);
}

inline static u32 c11(board_t* bd)
{
	return IND15(10,25,40,55,70,85,100,115,130,145,160,175,190,205,220);
}

inline static u32 c12(board_t* bd)
{
	return IND15(11,26,41,56,71,86,101,116,131,146,161,176,191,206,221);
}

inline static u32 c13(board_t* bd)
{
	return IND15(12,27,42,57,72,87,102,117,132,147,162,177,192,207,222);
}

inline static u32 c14(board_t* bd)
{
	return IND15(13,28,43,58,73,88,103,118,133,148,163,178,193,208,223);
}

inline static u32 c15(board_t* bd)
{
	return IND15(14,29,44,59,74,89,104,119,134,149,164,179,194,209,224);
}

// return main diagoanl indices
// md1 -> md29 corresponds to upper right -> lower left main diagonals
inline static u32 md1(board_t* bd)
{
	return 0;
}

inline static u32 md2(board_t* bd)
{
	return 0;
}

inline static u32 md3(board_t* bd)
{
	return 0;
}

inline static u32 md4(board_t* bd)
{
	return 0;
}

inline static u32 md5(board_t* bd)
{
	return IND5(10,26,42,58,74);
}

inline static u32 md6(board_t* bd)
{
	return IND6(9,25,41,57,73,89);
}

inline static u32 md7(board_t* bd)
{
	return IND7(8,24,40,56,72,88,104);
}

inline static u32 md8(board_t* bd)
{
	return IND8(7,23,39,55,71,87,103,119);
}

inline static u32 md9(board_t* bd)
{
	return IND9(6,22,38,54,70,86,102,118,134);
}

inline static u32 md10(board_t* bd)
{
	return IND10(5,21,37,53,69,85,101,117,133,149);
}

inline static u32 md11(board_t* bd)
{
	return IND11(4,20,36,52,68,84,100,116,132,148,164);
}

inline static u32 md12(board_t* bd)
{
	return IND12(3,19,35,51,67,83,99,115,131,147,163,179);
}

inline static u32 md13(board_t* bd)
{
	return IND13(2,18,34,50,66,82,98,114,130,146,162,178,194);
}

inline static u32 md14(board_t* bd)
{
	return IND14(1,17,33,49,65,81,97,113,129,145,161,177,193,209);
}

inline static u32 md15(board_t* bd)
{
	return IND15(0,16,32,48,64,80,96,112,128,144,160,176,192,208,224);
}

inline static u32 md16(board_t* bd)
{
	return IND14(15,31,47,63,79,95,111,127,143,159,175,191,207,223);
}

inline static u32 md17(board_t* bd)
{
	return IND13(30,46,62,78,94,110,126,142,158,174,190,206,222);
}

inline static u32 md18(board_t* bd)
{
	return IND12(45,61,77,93,109,125,141,157,173,189,205,221);
}

inline static u32 md19(board_t* bd)
{
	return IND11(60,76,92,108,124,140,156,172,188,204,220);
}

inline static u32 md20(board_t* bd)
{
	return IND10(75,91,107,123,139,155,171,187,203,219);
}

inline static u32 md21(board_t* bd)
{
	return IND9(90,106,122,138,154,170,186,202,218);
}

inline static u32 md22(board_t* bd)
{
	return IND8(105,121,137,153,169,185,201,217);
}

inline static u32 md23(board_t* bd)
{
	return IND7(120,136,152,168,184,200,216);
}

inline static u32 md24(board_t* bd)
{
	return IND6(135,151,167,183,199,215);
}

inline static u32 md25(board_t* bd)
{
	return IND5(150,166,182,198,214);
}

inline static u32 md26(board_t* bd)
{
	return 0;
}

inline static u32 md27(board_t* bd)
{
	return 0;
}

inline static u32 md28(board_t* bd)
{
	return 0;
}

inline static u32 md29(board_t* bd)
{
	return 0;
}

// return anti-diagonal indices
// ad1 -> ad29 corresponds to upper left -> lower right anti-diagonals
inline static u32 ad1(board_t* bd)
{
	return 0;
}

inline static u32 ad2(board_t* bd)
{
	return 0;
}

inline static u32 ad3(board_t* bd)
{
	return 0;
}

inline static u32 ad4(board_t* bd)
{
	return 0;
}

inline static u32 ad5(board_t* bd)
{
	return IND5(4,18,32,46,60);
}

inline static u32 ad6(board_t* bd)
{
	return IND6(5,19,33,47,61,75);
}

inline static u32 ad7(board_t* bd)
{
	return IND7(6,20,34,48,62,76,90);
}

inline static u32 ad8(board_t* bd)
{
	return IND8(7,21,35,49,63,77,91,105);
}

inline static u32 ad9(board_t* bd)
{
	return IND9(8,22,36,50,64,78,92,106,120);
}

inline static u32 ad10(board_t* bd)
{
	return IND10(9,23,37,51,65,79,93,107,121,135);
}

inline static u32 ad11(board_t* bd)
{
	return IND11(10,24,38,52,66,80,94,108,122,136,150);
}

inline static u32 ad12(board_t* bd)
{
	return IND12(11,25,39,53,67,81,95,109,123,137,151,165);
}

inline static u32 ad13(board_t* bd)
{
	return IND13(12,26,40,54,68,82,96,110,124,138,152,166,180);
}

inline static u32 ad14(board_t* bd)
{
	return IND14(13,27,41,55,69,83,97,111,125,139,153,167,181,195);
}

inline static u32 ad15(board_t* bd)
{
	return IND15(14,28,42,56,70,84,98,112,126,140,154,168,182,196,210);
}

inline static u32 ad16(board_t* bd)
{
	return IND14(29,43,57,71,85,99,113,127,141,155,169,183,197,211);
}

inline static u32 ad17(board_t* bd)
{
	return IND13(44,58,72,86,100,114,128,142,156,170,184,198,212);
}

inline static u32 ad18(board_t* bd)
{
	return IND12(59,73,87,101,115,129,143,157,171,185,199,213);
}

inline static u32 ad19(board_t* bd)
{
	return IND11(74,88,102,116,130,144,158,172,186,200,214);
}

inline static u32 ad20(board_t* bd)
{
	return IND10(89,103,117,131,145,159,173,187,201,215);
}

inline static u32 ad21(board_t* bd)
{
	return IND9(104,118,132,146,160,174,188,202,216);
}

inline static u32 ad22(board_t* bd)
{
	return IND8(119,133,147,161,175,189,203,217);
}

inline static u32 ad23(board_t* bd)
{
	return IND7(134,148,162,176,190,204,218);
}

inline static u32 ad24(board_t* bd)
{
	return IND6(149,163,177,191,205,219);
}

inline static u32 ad25(board_t* bd)
{
	return IND5(164,178,192,206,220);
}

inline static u32 ad26(board_t* bd)
{
	return 0;
}

inline static u32 ad27(board_t* bd)
{
	return 0;
}

inline static u32 ad28(board_t* bd)
{
	return 0;
}

inline static u32 ad29(board_t* bd)
{
	return 0;
}

static u32 (*r[15 * 15])(board_t* bd) = {
	r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,
	r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,
	r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,
	r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,
	r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,
	r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,
	r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,
	r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,
	r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,
	r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10,
	r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11,
	r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12,
	r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13,
	r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14,
	r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15
};

static u32 (*c[15 * 15])(board_t* bd) = {
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15
};

static u32 (*md[15 * 15])(board_t* bd) = {
	md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4, md3, md2, md1,
	md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4, md3, md2,
	md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4, md3,
	md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4,
	md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5,
	md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6,
	md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7,
	md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8,
	md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9,
	md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,
	md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,
	md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,
	md27,md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,
	md28,md27,md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,
	md29,md28,md27,md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15
};

static u32 (*ad[15 * 15])(board_t* bd) = {
	ad1, ad2, ad3, ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,
	ad2, ad3, ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,
	ad3, ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,
	ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,
	ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,
	ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,
	ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,
	ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,
	ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,
	ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,
	ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,
	ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,
	ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,ad27,
	ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,ad27,ad28,
	ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,ad27,ad28,ad29
};

static u8 md_len[15 * 15] = {
	15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,
	14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,
	13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,
	12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,
	11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,
	10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,
	9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,
	8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,
	7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,
	6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10,
	5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11,
	4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12,
	3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13,
	2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14,
	1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15
};

static u8 ad_len[15 * 15] = {
	1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 
	2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14,
	3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13,
	4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12,
	5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11,
	6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10,
	7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,
	8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,
	9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,
	10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,
	11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,
	12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,
	13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,
	14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,
	15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1
};

/*******************************************************************************
						board_t operation implementation
*******************************************************************************/
#define MAINDIAG	0
#define ANTIDIAG	1
#define SUBTRACT	0
#define ADD			1

// helper function
static inline void diag_helper( board_t* bd, const u8 which,
								const u8 op, const u8 pos)
{
	if(which == MAINDIAG && op == SUBTRACT)
	{
		switch(md_len[pos])
		{
		case 15:
			pattern_sub(pat(bd), pat(bd), &table15[(*md[pos])(bd)]);
			break;
		case 14:
			pattern_sub(pat(bd), pat(bd), &table14[(*md[pos])(bd)]);
			break;
		case 13:
			pattern_sub(pat(bd), pat(bd), &table13[(*md[pos])(bd)]);
			break;
		case 12:
			pattern_sub(pat(bd), pat(bd), &table12[(*md[pos])(bd)]);
			break;
		case 11:
			pattern_sub(pat(bd), pat(bd), &table11[(*md[pos])(bd)]);
			break;
		case 10:
			pattern_sub(pat(bd), pat(bd), &table10[(*md[pos])(bd)]);
			break;
		case 9:
			pattern_sub(pat(bd), pat(bd), &table9[(*md[pos])(bd)]);
			break;
		case 8:
			pattern_sub(pat(bd), pat(bd), &table8[(*md[pos])(bd)]);
			break;
		case 7:
			pattern_sub(pat(bd), pat(bd), &table7[(*md[pos])(bd)]);
			break;
		case 6:
			pattern_sub(pat(bd), pat(bd), &table6[(*md[pos])(bd)]);
			break;
		case 5:
			pattern_sub(pat(bd), pat(bd), &table5[(*md[pos])(bd)]);
			break;
		default:
			break;
		}
	}

	if(which == ANTIDIAG && op == SUBTRACT)
	{
		switch(ad_len[pos])
		{
		case 15:
			pattern_sub(pat(bd), pat(bd), &table15[(*ad[pos])(bd)]);
			break;
		case 14:
			pattern_sub(pat(bd), pat(bd), &table14[(*ad[pos])(bd)]);
			break;
		case 13:
			pattern_sub(pat(bd), pat(bd), &table13[(*ad[pos])(bd)]);
			break;
		case 12:
			pattern_sub(pat(bd), pat(bd), &table12[(*ad[pos])(bd)]);
			break;
		case 11:
			pattern_sub(pat(bd), pat(bd), &table11[(*ad[pos])(bd)]);
			break;
		case 10:
			pattern_sub(pat(bd), pat(bd), &table10[(*ad[pos])(bd)]);
			break;
		case 9:
			pattern_sub(pat(bd), pat(bd), &table9[(*ad[pos])(bd)]);
			break;
		case 8:
			pattern_sub(pat(bd), pat(bd), &table8[(*ad[pos])(bd)]);
			break;
		case 7:
			pattern_sub(pat(bd), pat(bd), &table7[(*ad[pos])(bd)]);
			break;
		case 6:
			pattern_sub(pat(bd), pat(bd), &table6[(*ad[pos])(bd)]);
			break;
		case 5:
			pattern_sub(pat(bd), pat(bd), &table5[(*ad[pos])(bd)]);
			break;
		default:
			break;
		}
	}

	if(which == MAINDIAG && op == ADD)
	{
		switch(md_len[pos])
		{
		case 15:
			pattern_add(pat(bd), pat(bd), &table15[(*md[pos])(bd)]);
			break;
		case 14:
			pattern_add(pat(bd), pat(bd), &table14[(*md[pos])(bd)]);
			break;
		case 13:
			pattern_add(pat(bd), pat(bd), &table13[(*md[pos])(bd)]);
			break;
		case 12:
			pattern_add(pat(bd), pat(bd), &table12[(*md[pos])(bd)]);
			break;
		case 11:
			pattern_add(pat(bd), pat(bd), &table11[(*md[pos])(bd)]);
			break;
		case 10:
			pattern_add(pat(bd), pat(bd), &table10[(*md[pos])(bd)]);
			break;
		case 9:
			pattern_add(pat(bd), pat(bd), &table9[(*md[pos])(bd)]);
			break;
		case 8:
			pattern_add(pat(bd), pat(bd), &table8[(*md[pos])(bd)]);
			break;
		case 7:
			pattern_add(pat(bd), pat(bd), &table7[(*md[pos])(bd)]);
			break;
		case 6:
			pattern_add(pat(bd), pat(bd), &table6[(*md[pos])(bd)]);
			break;
		case 5:
			pattern_add(pat(bd), pat(bd), &table5[(*md[pos])(bd)]);
			break;
		default:
			break;
		}
	}

	if(which == ANTIDIAG && op == ADD)
	{
		switch(ad_len[pos])
		{
		case 15:
			pattern_add(pat(bd), pat(bd), &table15[(*ad[pos])(bd)]);
			break;
		case 14:
			pattern_add(pat(bd), pat(bd), &table14[(*ad[pos])(bd)]);
			break;
		case 13:
			pattern_add(pat(bd), pat(bd), &table13[(*ad[pos])(bd)]);
			break;
		case 12:
			pattern_add(pat(bd), pat(bd), &table12[(*ad[pos])(bd)]);
			break;
		case 11:
			pattern_add(pat(bd), pat(bd), &table11[(*ad[pos])(bd)]);
			break;
		case 10:
			pattern_add(pat(bd), pat(bd), &table10[(*ad[pos])(bd)]);
			break;
		case 9:
			pattern_add(pat(bd), pat(bd), &table9[(*ad[pos])(bd)]);
			break;
		case 8:
			pattern_add(pat(bd), pat(bd), &table8[(*ad[pos])(bd)]);
			break;
		case 7:
			pattern_add(pat(bd), pat(bd), &table7[(*ad[pos])(bd)]);
			break;
		case 6:
			pattern_add(pat(bd), pat(bd), &table6[(*ad[pos])(bd)]);
			break;
		case 5:
			pattern_add(pat(bd), pat(bd), &table5[(*ad[pos])(bd)]);
			break;
		default:
			break;
		}
	}
}

void board_reset(board_t* bd)
{
	int i;

	bd->num = 0;
	mvlist_reset(mstk(bd));
	pattern_reset(pinc(bd));
	pattern_reset(hpinc(bd));

	for(i = 0; i < 15 * 15; i++)
	{
		bd->arr[i] = EMPTY;
		pattern_reset(&bd->pat[i]);
		mvlist_reset(&bd->mlist[i]);
		mvlist_reset(&bd->hlist[i]);
	}
}

void board_init(board_t* bd, const char (*arr)[15])
{
	int r, c;
	board_reset(bd);
	for(r = 0; r < 15; r++)
		for(c = 0; c < 15; c++)
			do_move(bd, r * 15 + c, arr[r][c]);
}

u8 board_gameover(const board_t* bd)
{
	if(bd->num == 15 * 15)
		return DRAW;

	if(pattern_read(pat(bd), FIVE, BLACK))
		return BLACK;
	
	if(pattern_read(pat(bd), FIVE, WHITE))
		return WHITE;

	if(isForbidden)
	{
		if(pattern_read(pat(bd), LONG, BLACK) || pattern_read(pat(bd), LONG, WHITE))
			return WHITE;

		if((pattern_read(pinc(bd),FREE4,BLACK)+pattern_read(pinc(bd),DEAD4,BLACK)>1)
		|| (pattern_read(pinc(bd),FREE3,BLACK)+pattern_read(pinc(bd),FREE3a,BLACK)>1))
			return WHITE;
	}

	return false;
}

void do_move_no_mvlist(board_t* bd, const u8 pos, const u8 color)
{
	if(bd->arr[pos] != EMPTY || color == EMPTY)
		return;

	bd->num++;

	// subtract critical line patterns from the old pattern
	pattern_copy(&bd->pat[bd->num - 1], pat(bd));
	pattern_sub(pat(bd), pat(bd), &table15[(*r[pos])(bd)]);
	pattern_sub(pat(bd), pat(bd), &table15[(*c[pos])(bd)]);
	diag_helper(bd, MAINDIAG, SUBTRACT, pos);
	diag_helper(bd, ANTIDIAG, SUBTRACT, pos);
	
	// make move
	bd->arr[pos] = color;
	mvlist_insert_back(mstk(bd), pos);

	// add new critical line patterns
	pattern_add(pat(bd), pat(bd), &table15[(*r[pos])(bd)]);
	pattern_add(pat(bd), pat(bd), &table15[(*c[pos])(bd)]);
	diag_helper(bd, MAINDIAG, ADD, pos);
	diag_helper(bd, ANTIDIAG, ADD, pos);

	// update pinc
	pattern_sub(hpinc(bd), pat(bd), &bd->pat[bd->num - 1]);
}

void do_move(board_t* bd, const u8 pos, const u8 color)
{
	if(bd->arr[pos] != EMPTY || color == EMPTY)
		return;
	
	bd->num++;

	// subtract old critical line patterns
	pattern_copy(&bd->pat[bd->num - 1], pat(bd));
	pattern_sub(pat(bd), pat(bd), &table15[(*r[pos])(bd)]);
	pattern_sub(pat(bd), pat(bd), &table15[(*c[pos])(bd)]);
	diag_helper(bd, MAINDIAG, SUBTRACT, pos);
	diag_helper(bd, ANTIDIAG, SUBTRACT, pos);
	
	// make move
	bd->arr[pos] = color;
	mvlist_insert_back(mstk(bd), pos);

	// add new critical line patterns
	pattern_add(pat(bd), pat(bd), &table15[(*r[pos])(bd)]);
	pattern_add(pat(bd), pat(bd), &table15[(*c[pos])(bd)]);
	diag_helper(bd, MAINDIAG, ADD, pos);
	diag_helper(bd, ANTIDIAG, ADD, pos);

	// update pinc
	pattern_sub(pinc(bd), pat(bd), &bd->pat[bd->num - 1]);

	// generate mvlist
	int i;
	mvlist_copy(&bd->mlist[bd->num - 1], mlist(bd));
	mvlist_remove(mlist(bd), pos);
	for(i = 0; i < NEI_SIZE; i++)
	{
		if(nei[pos][i] == INVALID)
			break;
		if(bd->arr[nei[pos][i]] == EMPTY)
			mvlist_insert_front(mlist(bd), nei[pos][i]);
	}
}

void undo(board_t* bd)
{
	if(bd->num == 0)
		return;
	else
	{
		bd->num--;
		bd->arr[mvlist_last(mstk(bd))] = EMPTY;
		mvlist_remove_back(mstk(bd));
	}
}

