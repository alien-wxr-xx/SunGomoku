/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * mvlist.h - implementation of mvlist_t data structure
 *
 * Similar to a double-ended linked list with no repetition.
 * The time complexity of each operation is O(1) if not mentioned.
 */

#ifndef __MVLIST_H__
#define __MVLIST_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"

#define HEAD	15 * 15
#define END		15 * 15 + 1

// node data structure
typedef struct {
	bool valid;		// existence of the node
	u8 next;		// next node index
	u8 prev;		// previous node index
} node_t;

// mvlist_t data structure
typedef struct {
	node_t arr[15 * 15 + 2];	// node array
	u8 size;					// # of nodes
} mvlist_t;

/*
 * Reset a mvlist. Time complexity O(15 * 15).
 */
static void mvlist_reset(mvlist_t* mv);

/*
 * Return the size of the mvlist.
 */
static u8 mvlist_size(const mvlist_t* mv);

/*
 * Return true if pos exists in the mvlist.
 */
static bool mvlist_find(const mvlist_t* mv, const u8 pos);

/*
 * Return the first position.
 */
static u8 mvlist_first(const mvlist_t* mv);

/*
 * Return the last position.
 */
static u8 mvlist_last(const mvlist_t* mv);

/*
 * Return the next position of pos.
 */
static u8 mvlist_next(const mvlist_t* mv, const u8 pos);

/* 
 * Insert pos at the front of the mvlist. Return true if inserted.
 */
static bool mvlist_insert_front(mvlist_t* mv, const u8 pos);

/*
 * Insert pos at the back of the mvlist. Return true if inserted.
 */
static bool mvlist_insert_back(mvlist_t* mv, const u8 pos);

/*
 * Remove pos from the mvlist. Return false if no such position.
 */
static bool mvlist_remove(mvlist_t* mv, const u8 pos);

/*
 * Remove and return the first position. Return INVALID if mvlist is empty.
 */
static u8 mvlist_remove_front(mvlist_t* mv);

/*
 * Remove and return the last position. Return INVALID if mvlist is empty.
 */
static u8 mvlist_remove_back(mvlist_t* mv);

/*
 * Remove all positions. Time complexity O(n);
 */
static void mvlist_remove_all(mvlist_t* mv);

/*
 * First remove all positions of output then copy input to output.
 * Time complexity O(n).
 */
static void mvlist_copy(const mvlist_t* in, mvlist_t* out);

// Implementation
static inline void mvlist_reset(mvlist_t* mv)
{
	int i;
	mv->arr[HEAD].prev = INVALID;
	mv->arr[HEAD].next = END;
	mv->arr[END].prev = HEAD;
	mv->arr[END].next = INVALID;
	mv->size = 0;
	for(i = 0; i < 15 * 15; i++)
		mv->arr[i].valid = false;
}

static inline u8 mvlist_size(const mvlist_t* mv)
{
	return mv->size;
}

static inline bool mvlist_find(const mvlist_t* mv, const u8 pos)
{
	return mv->arr[pos].valid;
}

static inline u8 mvlist_first(const mvlist_t* mv)
{
	return mv->arr[HEAD].next;
}

static inline u8 mvlist_last(const mvlist_t* mv)
{
	return mv->arr[END].prev;
}

static inline u8 mvlist_next(const mvlist_t* mv, const u8 pos)
{
	return mv->arr[pos].next;
}

static inline bool mvlist_insert_front(mvlist_t* mv, const u8 pos)
{
	if(mv->arr[pos].valid)
		return false;

	mv->arr[pos].prev = HEAD;
	mv->arr[pos].next = mv->arr[HEAD].next;
	mv->arr[HEAD].next = pos;
	mv->arr[mv->arr[pos].next].prev = pos;

	mv->arr[pos].valid = true;
	mv->size++;

	return true;
}

static inline bool mvlist_insert_back(mvlist_t* mv, const u8 pos)
{
	if(mv->arr[pos].valid)
		return false;
	
	mv->arr[pos].prev = mv->arr[END].prev;
	mv->arr[pos].next = END;
	mv->arr[END].prev = pos;
	mv->arr[mv->arr[pos].prev].next = pos;

	mv->arr[pos].valid = true;
	mv->size++;

	return true;
}

static inline bool mvlist_remove(mvlist_t* mv, const u8 pos)
{
	if(!mv->arr[pos].valid)
		return false;
	
	mv->arr[mv->arr[pos].prev].next = mv->arr[pos].next;
	mv->arr[mv->arr[pos].next].prev = mv->arr[pos].prev;

	mv->arr[pos].valid = false;
	mv->size--;

	return true;
}

static inline u8 mvlist_remove_front(mvlist_t* mv)
{
	u8 pos = mv->arr[HEAD].next;
	if(pos != END)
	{
		mvlist_remove(mv, pos);
		return pos;
	}
	return INVALID;
}

static inline u8 mvlist_remove_back(mvlist_t* mv)
{
	u8 pos = mv->arr[END].prev;
	if(pos != HEAD)
	{
		mvlist_remove(mv, pos);
		return pos;
	}
	return INVALID;
}

static inline void mvlist_remove_all(mvlist_t* mv)
{
	u8 pos = mv->arr[HEAD].next;
	while(pos != END)
	{
		mv->arr[pos].valid = false;
		pos = mv->arr[pos].next;
	}
	mv->arr[HEAD].next = END;
	mv->arr[END].prev = HEAD;
	mv->size = 0;
}

static inline void mvlist_copy(const mvlist_t* in, mvlist_t* out)
{
	mvlist_remove_all(out);
	u8 pos = mvlist_first(in);
	while(pos != END)
	{
		mvlist_insert_back(out, pos);
		pos = mvlist_next(in, pos);
	}
}

#ifdef  __cplusplus
}
#endif

#endif

