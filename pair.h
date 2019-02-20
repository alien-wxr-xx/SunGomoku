/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * pair.h - pair_t data structure
 */

#ifndef __PAIR_H__
#define __PAIR_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"
#include "mvlist.h"

// pos-key struct
typedef struct {
	u8 pos;
	long key;
} pr_t;

// pair_t data structure
typedef struct {
	pr_t arr[15 * 15];
	int cnt;
} pair_t;

// reset pair
void pair_reset(pair_t* pair);

// insert a pos-key pair to pair_t structure
void pair_insert(pair_t* pair, const u8 pos, const long key);

// sort first N elements of the pair_t array descending
// not in place
void pair_sort(pair_t* pair);

// sort first N elements of the pair_t array descending
// in place
void pair_sort_inplace(pair_t* pair);

void pair_to_mvlist(const pair_t* pair, mvlist_t* mv);

#ifdef  __cplusplus
}
#endif

#endif

