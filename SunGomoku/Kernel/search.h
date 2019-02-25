/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * search.h - heuristic search functions
 */

#ifndef __SEARCH_H__
#define __SEARCH_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"
#include "board.h"

// score structure
typedef struct {
	long free4;
	long dead4;
	long free3;
	long dead3;
	long free2;
	long dead2;
	long free1;
	long dead1;
	long free3a;
	long free2a;
	long free1a;
} score_t;

// search constant structure
typedef struct {
	score_t sc;		// score constants for searching
	u8 me;			// my color
	u8 opp;			// opponent's color
	u8 hpleaf;		// heuristic pre-search leaf size
	u8 htleaf;		// heuristic search game tree leaf size
	u8 dep;			// alpha-beta search depth
	float tlimit;	// time limit for one move in seconds
	bool presrh;	// if do shallower search first
	bool book;		// if use open book
} search_t;

/*
 * Return the score of the current board.
 *
 * @param[out]	bd		Pointer to board_t structure.
 * @param[in]	sc		The score_t structure.
 * @param[in]	me		My color.
 * @param[in]	opp		Opponent's color.
 * @param[in]	next	Next move color.
 */
long evaluate(const board_t* bd, const score_t* sc,	const u8 me, 
													const u8 opp, const u8 next);

/*
 * Generate hlist(bd).
 * Some forbidden points are not considered to avoid empty hlist.
 *
 * @param[out]	bd		Pointer to board_t structure.
 * @param[in]	srh		The search_t structure.
 * @param[in]	me		Next move color.
 * @param[in]	opp		The other color.
 */
void heuristic_generate(board_t* bd, const search_t* srh, const u8 me, const u8 opp);

/*
 * Alpha-beta search with heuristically generated moves
 *
 * @param[in]	bd		The current board.
 * @param[in]	srh		The search_t structure.
 * @param[in]	dep		Remaining search depth.
 * @param[in]	next	Next move color.
 * @param[out]	best	Pointer to the best move.
 * @param[in]	heu		Set if need to generate hlist(bd).
 *
 * @return	The score of the root node.
 */
long alphabeta(board_t* bd, const search_t* srh, u8 dep, const u8 next,
						long alpha, long beta, u8* best, const bool heu);
long alphabeta_time(board_t* bd, const search_t* srh, u8 dep, const u8 next,
							long alpha, long beta, u8* best, const bool heu);

/*
 * Return the best position to move.
 */
u8 heuristic(board_t* bd, const search_t* srh);

#ifdef  __cplusplus
}
#endif

#endif

