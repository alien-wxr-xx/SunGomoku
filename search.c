/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * search.c - heuristic search functions
 */

#include "search.h"
#include "macro.h"
#include "pair.h"
#include "board.h"
#include "book.h"

extern bool isForbidden;
static bool BookInUse = false;	// set if the opening book is in use.

static pair_t Pair;
static clock_t OriginTime, CurrentTime;

long evaluate(const board_t* bd, const score_t* sc,	const u8 me, 
													const u8 opp, const u8 next)
{
	long score = 0;
	u8 win = board_gameover(bd);

	if(me == BLACK)
	{
		if(win == BLACK)
			return WIN;
		else if(win == WHITE)
			return LOSE;
		else if(win == DRAW)
			return 0;
	}
	else if(me == WHITE)
	{
		if(win == BLACK)
			return LOSE;
		else if(win == WHITE)
			return WIN;
		else if(win == DRAW)
			return 0;
	}

	if(next == me)
	{
		score += 10000      * pattern_read(pat(bd), FREE4, me);
		score += 10000      * pattern_read(pat(bd), DEAD4, me);
		score += 5000       * pattern_read(pat(bd), FREE3, me);
		score += sc->dead3  * pattern_read(pat(bd), DEAD3, me);
		score += sc->free2  * pattern_read(pat(bd), FREE2, me);
		score += sc->dead2  * pattern_read(pat(bd), DEAD2, me);
		score += sc->free1  * pattern_read(pat(bd), FREE1, me);
		score += sc->dead1  * pattern_read(pat(bd), DEAD1, me);
		score += 5000       * pattern_read(pat(bd), FREE3a, me);
		score += sc->free2a * pattern_read(pat(bd), FREE2a, me);
		score += sc->free1a * pattern_read(pat(bd), FREE1a, me);

		score -= sc->free4  * pattern_read(pat(bd), FREE4, opp);
		score -= sc->dead4  * pattern_read(pat(bd), DEAD4, opp);
		score -= sc->free3  * pattern_read(pat(bd), FREE3, opp);
		score -= sc->dead3  * pattern_read(pat(bd), DEAD3, opp);
		score -= sc->free2  * pattern_read(pat(bd), FREE2, opp);
		score -= sc->dead2  * pattern_read(pat(bd), DEAD2, opp);
		score -= sc->free1  * pattern_read(pat(bd), FREE1, opp);
		score -= sc->dead1  * pattern_read(pat(bd), DEAD1, opp);
		score -= sc->free3a * pattern_read(pat(bd), FREE3a, opp);
		score -= sc->free2a * pattern_read(pat(bd), FREE2a, opp);
		score -= sc->free1a * pattern_read(pat(bd), FREE1a, opp);
	}
	else if(next == opp)
	{
		score += sc->free4  * pattern_read(pat(bd), FREE4, me);
		score += sc->dead4  * pattern_read(pat(bd), DEAD4, me);
		score += sc->free3  * pattern_read(pat(bd), FREE3, me);
		score += sc->dead3  * pattern_read(pat(bd), DEAD3, me);
		score += sc->free2  * pattern_read(pat(bd), FREE2, me);
		score += sc->dead2  * pattern_read(pat(bd), DEAD2, me);
		score += sc->free1  * pattern_read(pat(bd), FREE1, me);
		score += sc->dead1  * pattern_read(pat(bd), DEAD1, me);
		score += sc->free3a * pattern_read(pat(bd), FREE3a, me);
		score += sc->free2a * pattern_read(pat(bd), FREE2a, me);
		score += sc->free1a * pattern_read(pat(bd), FREE1a, me);

		score -= 10000      * pattern_read(pat(bd), FREE4, opp);
		score -= 10000      * pattern_read(pat(bd), DEAD4, opp);
		score -= 5000       * pattern_read(pat(bd), FREE3, opp);
		score -= sc->dead3  * pattern_read(pat(bd), DEAD3, opp);
		score -= sc->free2  * pattern_read(pat(bd), FREE2, opp);
		score -= sc->dead2  * pattern_read(pat(bd), DEAD2, opp);
		score -= sc->free1  * pattern_read(pat(bd), FREE1, opp);
		score -= sc->dead1  * pattern_read(pat(bd), DEAD1, opp);
		score -= 5000       * pattern_read(pat(bd), FREE3a, opp);
		score -= sc->free2a * pattern_read(pat(bd), FREE2a, opp);
		score -= sc->free1a * pattern_read(pat(bd), FREE1a, opp);
	}
	
	return score;
}

/*
 * Generate must-do moves in hlist(bd).
 *
 * @param [out]	bd		Pointer to board_t structure
 * @param [in]	me		Next move color
 * @param [in]	opp		The other color.
 */
static bool must_do_generate(board_t* bd, const u8 me, const u8 opp)
{
	u8 pos;

	// me has free4 or dead4, choose one position to win
	if(pattern_read(pat(bd), FREE4, me) || pattern_read(pat(bd), DEAD4, me))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);
			
			if(isForbidden)
			{
				if(me == BLACK)
				{
					if(pattern_read(hpinc(bd), FIVE, me))
					{
						mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
						undo(bd);
						return true;
					}
				}
				else if(me == WHITE)
				{
					if(pattern_read(hpinc(bd), FIVE, me)
					|| pattern_read(hpinc(bd), LONG, me))
					{
						mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
						undo(bd);
						return true;
					}
				}
			}
			else
			{
				if(pattern_read(hpinc(bd), FIVE, me)
				|| pattern_read(hpinc(bd), LONG, me))
				{
					mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
					undo(bd);
					return true;
				}
			}

			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}

	// opp has free4 and me has no four, choose one position to lose
	if(pattern_read(pat(bd), FREE4, opp))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if(pattern_read(hpinc(bd), FREE4, opp) < 0)
			{
				mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
				undo(bd);
				return true;
			}

			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}

	// opp has dead4 and me has no four, me must defend
	if(pattern_read(pat(bd), DEAD4, opp))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if(pattern_read(hpinc(bd), DEAD4, opp) < 0)
			{
				mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
				undo(bd);
				return true;
			}
			
			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}

	// me has free3 and both have no four, me can form free4
	// consider forbidden points for black
	if(pattern_read(pat(bd), FREE3, me) || pattern_read(pat(bd), FREE3a, me))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if(isForbidden)
			{
				if(me == BLACK)
				{
					if(pattern_read(hpinc(bd), FREE4, me) > 0
					&& (pattern_read(hpinc(bd), FREE4, me) 
					+ pattern_read(hpinc(bd), DEAD4, me) < 2)
					&& (pattern_read(hpinc(bd), FREE3, me) 
					+ pattern_read(hpinc(bd), FREE3a, me) < 2))
					{
						mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
						undo(bd);
						return true;
					}
				}
				else if(me == WHITE)
				{
					if(pattern_read(hpinc(bd), FREE4, me) > 0)
					{
						mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
						undo(bd);
						return true;
					}
				}
			}
			else
			{
				if(pattern_read(hpinc(bd), FREE4, me) > 0)
				{
					mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
					undo(bd);
					return true;
				}
			}

			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}

	// opp has free3 and me has no free3 and both have no four
	// me must form dead4 or defend
	if(pattern_read(pat(bd), FREE3, opp) || pattern_read(pat(bd), FREE3a, opp))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if((pattern_read(hpinc(bd),FREE3,opp)+pattern_read(hpinc(bd),FREE3a,opp)<0)
			|| (pattern_read(hpinc(bd),DEAD4,me) > 0))
				mvlist_insert_back(&bd->hlist[bd->num - 1], pos);
			
			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
		return true;
	}

	return false;
}

void heuristic_generate(board_t* bd, const search_t* srh, const u8 me, const u8 opp)
{
	pair_t pair;
	u8 pos, i;

	mvlist_remove_all(hlist(bd));

	if(bd->num == 0)
	{
		mvlist_insert_front(hlist(bd), 112);
		return;
	}

	if(must_do_generate(bd, me, opp))
		return;

	pair_reset(&pair);

	pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		do_move_no_mvlist(bd, pos, me);
		pair_insert(&pair, pos, evaluate(bd, &srh->sc, me, opp, opp));
		undo(bd);
		pos = mvlist_next(mlist(bd), pos);
	}
	
	pair_sort(&pair);

	if(pair.cnt < srh->htleaf)
		for(i = 0; i < pair.cnt; i++)
			mvlist_insert_back(hlist(bd), pair.arr[i].pos);
	else
		for(i = 0; i < srh->htleaf; i++)
			mvlist_insert_back(hlist(bd), pair.arr[i].pos);
}

// Notice: this function will mess up pinc(bd).
void heuristic_generate_root(board_t* bd, const search_t* srh,
							const u8 dep, const u8 me, const u8 opp)
{
	pair_t pair;
	u8 pos, i;

	mvlist_remove_all(hlist(bd));

	if(bd->num == 0)
	{
		mvlist_insert_front(hlist(bd), 112);
		return;
	}

	if(must_do_generate(bd, me, opp))
		return;

	pair_reset(&pair);

	pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		do_move(bd, pos, me);
		pair_insert(&pair, pos, alphabeta(bd, srh, dep-1, opp, LOSE-1, WIN+1, &i, 1));
		undo(bd);
		pos = mvlist_next(mlist(bd), pos);
	}

	pair_sort(&pair);

	if(pair.cnt < srh->hpleaf)
		for(i = 0; i < pair.cnt; i++)
			mvlist_insert_back(hlist(bd), pair.arr[i].pos);
	else
		for(i = 0; i < srh->hpleaf; i++)
			mvlist_insert_back(hlist(bd), pair.arr[i].pos);
}

long alphabeta(board_t* bd, const search_t* srh, u8 dep, const u8 next,
						long alpha, long beta, u8* best, const bool heu)
{
	long val;
	u8 pos, tmp;
	tmp = board_gameover(bd);

	if(tmp == srh->me)
		return WIN - bd->num;
	if(tmp == srh->opp)
		return LOSE + bd->num;
	if(tmp == DRAW)
		return 0;
	if(dep <= 0)
		return evaluate(bd, &srh->sc, srh->me, srh->opp, srh->me);

	// min node
	if(next == srh->opp)
	{
		if(dep > 1)
		{
			if(heu)
				heuristic_generate(bd, srh, srh->opp, srh->me);
			pos = mvlist_first(hlist(bd));
		}
		else
			pos = mvlist_first(mlist(bd));

		while(pos != END)
		{
			if(dep > 1)
				do_move(bd, pos, srh->opp);
			else
				do_move_no_mvlist_pinc(bd, pos, srh->opp);

			val = alphabeta(bd, srh, dep - 1, srh->me, alpha, beta, &tmp, 1);
			undo(bd);

			if(val < beta)
			{
				beta = val;
				*best = pos;
			}
			if(beta <= alpha)
				break;

			if(dep > 1)
				pos = mvlist_next(hlist(bd), pos);
			else
				pos = mvlist_next(mlist(bd), pos);
		}
		return beta;
	}

	// max node	
	if(next == srh->me)
	{
		if(dep > 1)
		{
			if(heu)
				heuristic_generate(bd, srh, srh->me, srh->opp);
			pos = mvlist_first(hlist(bd));
		}
		else
			pos = mvlist_first(mlist(bd));

		while(pos != END)
		{
			if(dep > 1)
				do_move(bd, pos, srh->me);
			else
				do_move_no_mvlist_pinc(bd, pos, srh->me);

			val = alphabeta(bd, srh, dep - 1, srh->opp,	alpha, beta, &tmp, 1);
			if(!heu)
				pair_insert(&Pair, pos, val);
			undo(bd);

			if(val > alpha)
			{
				alpha = val;
				*best = pos;
			}
			if(alpha >= beta)
				break;
			
			if(dep > 1)
				pos = mvlist_next(hlist(bd), pos);
			else
				pos = mvlist_next(mlist(bd), pos);
		}
		return alpha;
	}
	return INVALID;
}

long alphabeta_time(board_t* bd, const search_t* srh, u8 dep, const u8 next,
							long alpha, long beta, u8* best, const bool heu)
{
	// timeout check
	CurrentTime = clock();
	if(CurrentTime - OriginTime >= srh->tlimit * CLOCKS_PER_SEC)
		return LOSE;

	// gameover check
	long val;
	u8 pos, tmp;
	tmp = board_gameover(bd);

	if(tmp == srh->me)
		return WIN - bd->num;
	if(tmp == srh->opp)
		return LOSE + bd->num;
	if(tmp == DRAW)
		return 0;
	if(dep <= 0)
		return evaluate(bd, &srh->sc, srh->me, srh->opp, srh->me);

	// min node
	if(next == srh->opp)
	{
		if(dep > 1)
		{
			if(heu)
				heuristic_generate(bd, srh, srh->opp, srh->me);
			pos = mvlist_first(hlist(bd));
		}
		else
			pos = mvlist_first(mlist(bd));

		while(pos != END)
		{
			if(dep > 1)
				do_move(bd, pos, srh->opp);
			else
				do_move_no_mvlist_pinc(bd, pos, srh->opp);

			val = alphabeta_time(bd, srh, dep - 1, srh->me, alpha, beta, &tmp, 1);
			undo(bd);

			if(val < beta)
			{
				beta = val;
				*best = pos;
			}
			if(beta <= alpha)
				break;

			if(dep > 1)
				pos = mvlist_next(hlist(bd), pos);
			else
				pos = mvlist_next(mlist(bd), pos);
		}
		return beta;
	}

	// max node	
	if(next == srh->me)
	{
		if(dep > 1)
		{
			if(heu)
				heuristic_generate(bd, srh, srh->me, srh->opp);
			pos = mvlist_first(hlist(bd));
		}
		else
			pos = mvlist_first(mlist(bd));

		while(pos != END)
		{
			if(dep > 1)
				do_move(bd, pos, srh->me);
			else
				do_move_no_mvlist_pinc(bd, pos, srh->me);

			val = alphabeta_time(bd, srh, dep - 1, srh->opp, alpha, beta, &tmp, 1);
			if(!heu)
				pair_insert(&Pair, pos, val);
			undo(bd);

			if(val > alpha)
			{
				alpha = val;
				*best = pos;
			}
			if(alpha >= beta)
				break;
			
			if(dep > 1)
				pos = mvlist_next(hlist(bd), pos);
			else
				pos = mvlist_next(mlist(bd), pos);
		}
		return alpha;
	}
	return INVALID;
}

u8 heuristic(board_t* bd, const search_t* srh)
{
	u8 res, tmp;
	
	// first move
	if(bd->num == 0)
		return 112;

	// ai plays black and uses opening book
	if(srh->me == BLACK && srh->book)
	{
		if(bd->num == 2)
		{
			tmp = mvlist_next(mstk(bd), mvlist_first(mstk(bd)));

			if(tmp == 97 || tmp == 111 || tmp == 127 || tmp == 113)
			{
				book_choose_direct();
				BookInUse = true;
				book_generate(bd);
				return mvlist_first(hlist(bd));
			}
			else if(tmp == 96 || tmp == 126 || tmp == 128 || tmp == 98)
			{
				book_choose_indirect();
				BookInUse = true;
				book_generate(bd);
				return mvlist_first(hlist(bd));
			}
		}

		else if(BookInUse)
		{
			if(!book_generate(bd))
				BookInUse = false;
			else
			{
				tmp = mvlist_first(hlist(bd));

				do_move(bd, tmp, srh->me);
				if(board_gameover(bd) != WHITE)		// prevent sudden lose
				{
					undo(bd);
					return tmp;
				}
				else
					undo(bd);
			}
		}
	}

	// do the second move randomly when ai plays white
	else if(srh->me == WHITE && mvlist_first(mstk(bd)) == 112 && bd->num == 1)
	{
		BookInUse = false;
		tmp = rand() % 8;
		switch(tmp)
		{
			case 0:
				return 97;
			case 1:
				return 96;
			case 2:
				return 111;
			case 3:
				return 126;
			case 4:
				return 127;
			case 5:
				return 128;
			case 6:
				return 113;
			case 7:
				return 98;
			default:
				break;
		}
	}

	OriginTime = clock();
	if(srh->dep >= 8 && srh->presrh)
	{
		// pre-search to generate hlist
		heuristic_generate_root(bd, srh, srh->dep - 6, srh->me, srh->opp);

		// return if there is only one move
		if(mvlist_size(hlist(bd)) <= 1)
			return mvlist_first(hlist(bd));
	
		// basic search
		pair_reset(&Pair);
		pattern_reset(pinc(bd));
		alphabeta(bd, srh, srh->dep, srh->me, LOSE - 1, WIN + 1, &res, 0);
		pair_sort_inplace(&Pair);
		pair_to_mvlist(&Pair, hlist(bd));
		tmp = Pair.arr[0].pos;
	
		CurrentTime = clock();
		// return if disc number is less than 5
		if(bd->num <= 5)
			return Pair.arr[0].pos;
		// return if more than half of the time is used
		if(CurrentTime - OriginTime >= srh->tlimit * CLOCKS_PER_SEC / 2)
			return Pair.arr[0].pos;
		
		// deeper search
		pair_reset(&Pair);
		pattern_reset(pinc(bd));
		alphabeta_time(bd, srh, srh->dep + 2, srh->me, LOSE - 1, WIN + 1, &res, 0);

		if(Pair.cnt == 0)
			return tmp;
		else
		{
			pair_sort_inplace(&Pair);
			return Pair.arr[0].pos;
		}
	}
	else
	{
		pattern_reset(pinc(bd));
		alphabeta(bd, srh, srh->dep, srh->me, LOSE - 1, WIN + 1, &res, 1);
		return res;
	}
}

