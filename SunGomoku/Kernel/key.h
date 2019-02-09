/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * key.h - key data structure implememtation
 */

#ifndef __KEY_H__
#define __KEY_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"
#include "mvlist.h"

// key structure
typedef struct {
	u64 b1;
	u64 b2;
	u64 b3;
	u64 b4;
	u64 w1;
	u64 w2;
	u64 w3;
	u64 w4;
} key_t;

/*
 * Reset all structure members to all 0.
 */
static void key_reset(key_t* key);

/*
 * Return true if k1 is the same as k2. Else return false.
 */
static bool key_issame(const key_t* k1, const key_t* k2);

/*
 * Set a bit of key according to the input.
 */
static void key_setbit(key_t* key, const u8 pos, const u8 color);

/*
 * Generate key from the mvlist.
 */
static void key_gen_mvlist(key_t* key, const mvlist_t* mv);

/*
 * Generate key from the array.
 */
static void key_gen_arr(key_t* key, const u8* arr, const int N);

// Implementation
static u64 mask[64] = {
	0x8000000000000000, 0x4000000000000000,	0x2000000000000000,	0x1000000000000000,
	0x0800000000000000, 0x0400000000000000,	0x0200000000000000,	0x0100000000000000,
	0x0080000000000000, 0x0040000000000000,	0x0020000000000000,	0x0010000000000000,
	0x0008000000000000, 0x0004000000000000,	0x0002000000000000,	0x0001000000000000,
	0x0000800000000000, 0x0000400000000000,	0x0000200000000000,	0x0000100000000000,
	0x0000080000000000, 0x0000040000000000,	0x0000020000000000,	0x0000010000000000,
	0x0000008000000000, 0x0000004000000000,	0x0000002000000000,	0x0000001000000000,
	0x0000000800000000, 0x0000000400000000,	0x0000000200000000,	0x0000000100000000,
	0x0000000080000000, 0x0000000040000000,	0x0000000020000000,	0x0000000010000000,
	0x0000000008000000, 0x0000000004000000,	0x0000000002000000,	0x0000000001000000,
	0x0000000000800000, 0x0000000000400000,	0x0000000000200000,	0x0000000000100000,
	0x0000000000080000, 0x0000000000040000,	0x0000000000020000,	0x0000000000010000,
	0x0000000000008000, 0x0000000000004000,	0x0000000000002000,	0x0000000000001000,
	0x0000000000000800, 0x0000000000000400,	0x0000000000000200,	0x0000000000000100,
	0x0000000000000080, 0x0000000000000040,	0x0000000000000020,	0x0000000000000010,
	0x0000000000000008, 0x0000000000000004,	0x0000000000000002,	0x0000000000000001
};

static inline void key_reset(key_t* key)
{
	key->b1 = 0;
	key->b2 = 0;
	key->b3 = 0;
	key->b4 = 0;
	key->w1 = 0;
	key->w2 = 0;
	key->w3 = 0;
	key->w4 = 0;
}

static inline bool key_issame(const key_t* k1, const key_t* k2)
{
	if((k1->b1 == k2->b1 && k1->b2 == k2->b2 && k1->b3 == k2->b3 && k1->b4 == k2->b4)
	&& (k1->w1 == k2->w1 && k1->w2 == k2->w2 && k1->w3 == k2->w3 && k1->w4 == k2->w4))
		return true;
	else
		return false;
}

static inline void key_setbit(key_t* key, const u8 pos, const u8 color)
{
	if(color == BLACK)
	{
		if(pos < 64)
			key->b1 |= mask[pos];
		else if(pos < 128)
			key->b2 |= mask[pos - 64];
		else if(pos < 192)
			key->b3 |= mask[pos - 128];
		else if(pos < 256)
			key->b4 |= mask[pos - 192];
	}
	else if(color == WHITE)
	{
		if(pos < 64)
			key->w1 |= mask[pos];
		else if(pos < 128)
			key->w2 |= mask[pos - 64];
		else if(pos < 192)
			key->w3 |= mask[pos - 128];
		else if(pos < 256)
			key->w4 |= mask[pos - 192];
	}
}

static inline void key_gen_mvlist(key_t* key, const mvlist_t* mv)
{
	u8 color = BLACK;
	u8 pos;

	key_reset(key);
	pos = mvlist_first(mv);

	while(pos != END)
	{
		key_setbit(key, pos, color);
		if(color == BLACK)
			color = WHITE;
		else if(color == WHITE)
			color = BLACK;
		pos = mvlist_next(mv, pos);
	}
}

static inline void key_gen_arr(key_t* key, const u8* arr, const int N)
{
	int i;
	key_reset(key);

	for(i = 0; i < N; i++)
	{
		if(i % 2 == 0)
			key_setbit(key, arr[i], BLACK);
		else if(i % 2 == 1)
			key_setbit(key, arr[i], WHITE);
	}
}

#ifdef  __cplusplus
}
#endif

#endif

