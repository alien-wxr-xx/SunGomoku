/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * pattern.h - pattern_t data structure implementation
 */

#ifndef __PATTERN_H__
#define __PATTERN_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"

// TYPE macros
#define LONG		0
#define FIVE		1
#define FREE4		2
#define DEAD4		3
#define FREE3		4
#define DEAD3		5
#define FREE2		6
#define DEAD2		7
#define FREE1		8
#define DEAD1		9
#define FREE3a		10
#define FREE2a		11
#define FREE1a		12
#define PAT_NUM		13

// pattern_t data structure
typedef	struct {
	char black[PAT_NUM];
	char white[PAT_NUM];
} pattern_t;

// Reset a pattern.
static void pattern_reset(pattern_t* pat);

// Copy a pattern.
static void pattern_copy(const pattern_t* in, pattern_t* out);

// Return the value of type in the pattern.
static int pattern_read(const pattern_t* pat, const u8 type, const u8 color);

// Increase the value of type in the pattern.
static void pattern_inc(pattern_t* pat, const u8 type, const u8 color);

// Decrease the value of type in the pattern.
static void pattern_dec(pattern_t* pat, const u8 type, const u8 color);

// Add two patterns.
static void pattern_add(pattern_t* out, const pattern_t* in1, const pattern_t* in2);

// Subtract one pattern from another.
static void pattern_sub(pattern_t* out, const pattern_t* in1, const pattern_t* in2);

// Implementation
static inline
void pattern_reset(pattern_t* pat)
{
	int i;
	for(i = 0; i < PAT_NUM; i++)
	{
		pat->black[i] = 0;
		pat->white[i] = 0;
	}
}

static inline
void pattern_copy(const pattern_t* in, pattern_t* out)
{
	int i;
	for(i = 0; i < PAT_NUM; i++)
	{
		out->black[i] = in->black[i];
		out->white[i] = in->white[i];
	}
}

static inline
int pattern_read(const pattern_t* pat, const u8 type, const u8 color)
{
	if(color == BLACK)
		return pat->black[type];
	else if(color == WHITE)
		return pat->white[type];
	else
		return INVALID;
}

static inline
void pattern_inc(pattern_t* pat, const u8 type, const u8 color)
{
	if(color == BLACK)
		pat->black[type]++;
	else if(color == WHITE)
		pat->white[type]++;
	else
		return;
}

static inline
void pattern_dec(pattern_t* pat, const u8 type, const u8 color)
{
	if(color == BLACK)
		pat->black[type]--;
	else if(color == WHITE)
		pat->white[type]--;
	else
		return;
}

static inline
void pattern_add(pattern_t* out, const pattern_t* in1, const pattern_t* in2)
{
	int i;
	for(i = 0; i < PAT_NUM; i++)
	{
		out->black[i] = in1->black[i] + in2->black[i];
		out->white[i] = in1->white[i] + in2->white[i];
	}
}

static inline
void pattern_sub(pattern_t* out, const pattern_t* in1, const pattern_t* in2)
{
	int i;
	for(i = 0; i < PAT_NUM; i++)
	{
		out->black[i] = in1->black[i] - in2->black[i];
		out->white[i] = in1->white[i] - in2->white[i];
	}
}

#ifdef  __cplusplus
}
#endif

#endif

