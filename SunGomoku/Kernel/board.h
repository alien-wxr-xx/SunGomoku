/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * board.h - board_t data structure
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "pattern.h"
#include "mvlist.h"
#include "pair.h"
#include "table.h"

#define mstk(bd)	&bd->mstk
#define pinc(bd)	&bd->pinc
#define hpinc(bd)	&bd->hpinc
#define pat(bd)		&bd->pat[bd->num]
#define mlist(bd)	&bd->mlist[bd->num]
#define hlist(bd)	&bd->hlist[bd->num]

// board_t data structure
typedef struct {
	u8 num;							// # of discs
	u8 arr[15 * 15];				// disc array
	mvlist_t mstk;					// move stack
	pattern_t pinc;					// pattern increment for do_move
	pattern_t hpinc;				// pattern increment for do_move_no_mvlist
	pattern_t pat[15 * 15];			// pattern stack
	mvlist_t mlist[15 * 15];		// mvlist stack
	mvlist_t hlist[15 * 15];		// heuristic mvlist stack
} board_t;

/*
 * Generate neighbor lookup table.
 */
void nei_table_init();

/*
 * Generate pattern lookup tables.
 */
void pattern_table_init1();
void pattern_table_init2();

/*
 * Reset a board.
 */
void board_reset(board_t* bd);

/*
 * Set a board from a 15 * 15 array.
 */
void board_init(board_t* bd, const char (*arr)[15]);

/*
 * Return the win side if game is over or return false.
 */
u8 board_gameover(const board_t* bd);

/*
 * Make a move and update pat(bd), pinc(bd) and mlist(bd).
 */
void do_move(board_t* bd, const u8 pos, const u8 color);

/*
 * Make a move and update pat(bd) and hpinc(bd).
 */
void do_move_no_mvlist(board_t* bd, const u8 pos, const u8 color);

/*
 * Make a move and update pat(bd) and pinc(bd).
 */
void do_move_no_mvlist_pinc(board_t* bd, const u8 pos, const u8 color);

/*
 * Undo the most recent move.
 */
void undo(board_t* bd);

#ifdef  __cplusplus
}
#endif

#endif

