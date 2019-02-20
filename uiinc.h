/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * uiinc.h - interface functions for ui
 *
 * Notice: User can use macro EMPTY(0), BLACK(1), WHITE(2), DRAW(225) and false(0).
 */

#ifndef __UIINC_H__
#define __UIINC_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"

/*
 * Call this functions at the beginning of the program.
 */
void initialize();

/*
 * Call this function at the beginning of each round.
 */
void restart();

/*
 * Call this function at the end of the program.
 */
void uninitialize();

/*
 * Set if the rule contains forbidden point judgement.
 *
 * Usage: set_forbidden(1);		// consider forbidden points
 *		  set_forbidden(0);		// neglect forbidden points
 */
void set_forbidden(const int flag);

/*
 * Set game difficulty.
 *
 * Usage: set_difficulty(0);	// sb mode
 *		  set_difficulty(1);	// eazy mode
 *		  set_difficulty(2);	// normal mode
 */
void set_difficulty(const int dif);

/*
 * Do player's move.
 *
 * @param [in]	x, y	The row and column index of player's move.
 * @param [out]	isvoer	Set to BLACK(1) if black wins. Set to WHITE(2) if white wins.
 *						Set to DRAW(225) if draws. Else set to false(0).
 * @param [in]	color	Player's color, BLACK(1) or WHITE(2).
 *
 * Usage:	play_do_move(7, 7, &isover, BLACK);
 *			if(isover == BLACK)	black_win();
 *			else if(isover == WHITE) white_win();
 *			else if(isover == DRAW) draw();
 *			else resume();
 */
void player_do_move(const int x, const int y, int* isover, const u8 color);

/*
 * Do ai's move.
 *
 * @param [out]	isvoer	Set to BLACK(1) if black wins. Set to WHITE(2) if white wins.
 *						Set to DRAW(225) if draws. Else set to false(0).
 *
 * Return	The position ai moves.
 *
 * Usage:	int pos = ai_do_move(&isover, BLACK);
 *			if(isover == BLACK)	black_win();
 *			else if(isover == WHITE) white_win();
 *			else if(isover == DRAW) draw();
 *			else resume();
 *
 */
int ai_do_move(int* isover, const u8 color);

/*
 * Undo N moves.
 */
void undo_move(const int N);

#ifdef  __cplusplus
}
#endif

#endif

