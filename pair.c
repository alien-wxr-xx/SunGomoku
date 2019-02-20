/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * pair.c - pair_t data structure
 */

#include "macro.h" 
#include "pair.h"
#include "mvlist.h"

// position potential array
static u8 pot[15 * 15] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
	0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0,
	0, 1 ,2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0,
	0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0,
	0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// return true if a < b
static inline bool less(const pr_t* a, const pr_t* b)
{
	if(a->key < b->key)
		return true;
	else if(a->key > b->key)
		return false;
	else
	{
		if(pot[a->pos] < pot[b->pos])
			return true;
		else
			return false;
	}
}

// return true if a < b
static inline bool less_inplace(const pr_t* a, const pr_t* b)
{
	if(a->key < b->key)
		return true;
	else
		return false;
}

void pair_reset(pair_t* pair)
{
	pair->cnt = 0;
}

void pair_insert(pair_t* pair, const u8 pos, const long key)
{
	pair->arr[pair->cnt].pos = pos;
	pair->arr[pair->cnt].key = key;
	pair->cnt++;
}

void pair_sort(pair_t* pair)
{
	int i, j;
	pr_t tmp;

	for(i = 1; i < pair->cnt; i++)
	{
		for(j = i - 1; j >= 0; j--)
		{
			if(less(&pair->arr[j], &pair->arr[j + 1]))
			{
				tmp.pos = pair->arr[j].pos;
				tmp.key = pair->arr[j].key;
				pair->arr[j].pos = pair->arr[j + 1].pos;
				pair->arr[j].key = pair->arr[j + 1].key;
				pair->arr[j + 1].pos = tmp.pos;
				pair->arr[j + 1].key = tmp.key;
			}
			else
				break;
		}
	}
}

void pair_sort_inplace(pair_t* pair)
{
	int i, j;
	pr_t tmp;

	for(i = 1; i < pair->cnt; i++)
	{
		for(j = i - 1; j >= 0; j--)
		{
			if(less_inplace(&pair->arr[j], &pair->arr[j + 1]))
			{
				tmp.pos = pair->arr[j].pos;
				tmp.key = pair->arr[j].key;
				pair->arr[j].pos = pair->arr[j + 1].pos;
				pair->arr[j].key = pair->arr[j + 1].key;
				pair->arr[j + 1].pos = tmp.pos;
				pair->arr[j + 1].key = tmp.key;
			}
			else
				break;
		}
	}
}

void pair_to_mvlist(const pair_t* pair, mvlist_t* mv)
{
	int i;
	mvlist_remove_all(mv);
	for(i = 0; i < pair->cnt; i++)
		mvlist_insert_back(mv, pair->arr[i].pos);
}

