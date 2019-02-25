/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * search.c - implementation of heuristic searching
 */

#include "search.h"
#include "macro.h"
#include "board.h"
#include "book.h"

extern bool isForbidden;
static bool BookInUse = false;	// set if the opening book is in use.

/*******************************************************************************
							Helper variable and functions
*******************************************************************************/
// pos-key pair structure
typedef struct {
	u8 pos;
	long key;
} pair_t;

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
static inline bool less(pair_t* a, pair_t* b)
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

// sort first N elements of a pair_t array descending
static void pair_sort(pair_t* arr, u8 N)
{
	int i, j;
	pair_t tmp;

	for(i = 1; i < N; i++)
	{
		for(j = i - 1; j >= 0; j--)
		{
			if(less(&arr[j], &arr[j + 1]))
			{
				tmp.pos = arr[j].pos;
				tmp.key = arr[j].key;
				arr[j].pos = arr[j + 1].pos;
				arr[j].key = arr[j + 1].key;
				arr[j + 1].pos = tmp.pos;
				arr[j + 1].key = tmp.key;
			}
			else
				break;
		}
	}
}

/*******************************************************************************
								Heuristic functions
*******************************************************************************/
long evaluate(const board_t* bd, const score_t* sc, const u8 color)
{
	long score = 0;
	u8 win = board_gameover(bd);

	if(color == BLACK)
	{
		if(win == BLACK)
			return sc->win;
		else if(win == WHITE)
			return sc->lose;
		else if(win == DRAW)
			return 0;
	}
	else if(color == WHITE)
	{
		if(win == BLACK)
			return sc->lose;
		else if(win == WHITE)
			return sc->win;
		else if(win == DRAW)
			return 0;
	}
	else
		return INVALID;

	score += sc->free4 * pattern_read(pat(bd), FREE4, BLACK);
	score += sc->dead4 * pattern_read(pat(bd), DEAD4, BLACK);
	score += sc->free3 * pattern_read(pat(bd), FREE3, BLACK);
	score += sc->dead3 * pattern_read(pat(bd), DEAD3, BLACK);
	score += sc->free2 * pattern_read(pat(bd), FREE2, BLACK);
	score += sc->dead2 * pattern_read(pat(bd), DEAD2, BLACK);
	score += sc->free1 * pattern_read(pat(bd), FREE1, BLACK);
	score += sc->dead1 * pattern_read(pat(bd), DEAD1, BLACK);
	score += sc->free3a * pattern_read(pat(bd), FREE3a, BLACK);
	score += sc->free2a * pattern_read(pat(bd), FREE2a, BLACK);
	score += sc->free1a * pattern_read(pat(bd), FREE1a, BLACK);

	score -= sc->free4 * pattern_read(pat(bd), FREE4, WHITE);
	score -= sc->dead4 * pattern_read(pat(bd), DEAD4, WHITE);
	score -= sc->free3 * pattern_read(pat(bd), FREE3, WHITE);
	score -= sc->dead3 * pattern_read(pat(bd), DEAD3, WHITE);
	score -= sc->free2 * pattern_read(pat(bd), FREE2, WHITE);
	score -= sc->dead2 * pattern_read(pat(bd), DEAD2, WHITE);
	score -= sc->free1 * pattern_read(pat(bd), FREE1, WHITE);
	score -= sc->dead1 * pattern_read(pat(bd), DEAD1, WHITE);
	score -= sc->free3a * pattern_read(pat(bd), FREE3a, WHITE);
	score -= sc->free2a * pattern_read(pat(bd), FREE2a, WHITE);
	score -= sc->free1a * pattern_read(pat(bd), FREE1a, WHITE);

	if(color == BLACK)
		return score;
	else if(color == WHITE)
		return -score;

	return INVALID;
}

/*
 * Generate must-do moves in hlist(bd).
 *
 * @param [out]	bd		The hlist member of bd is changed.
 * @param [in]	me		My color.
 * @param [in]	opp		Opponent's color.
 */
static bool must_do_generate(board_t* bd, const u8 me, const u8 opp)
{
	u8 pos;

	// me has four so game ends next turn and only choose one place to win
	// notice: black shouldn't become long.
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
						mvlist_insert_front(&bd->hlist[bd->num - 1], pos);
						undo(bd);
						return true;
					}
				}
				else if(me == WHITE)
				{
					if(pattern_read(hpinc(bd), FIVE, me)
					|| pattern_read(hpinc(bd), LONG, me))
					{
						mvlist_insert_front(&bd->hlist[bd->num - 1], pos);
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
					mvlist_insert_front(&bd->hlist[bd->num - 1], pos);
					undo(bd);
					return true;
				}
			}

			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}

	// opp has free4 and me has no four so me loses next turn
	// only choose one place
	if(pattern_read(pat(bd), FREE4, opp))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if(pattern_read(hpinc(bd), FREE4, opp) < 0)
			{
				mvlist_insert_front(&bd->hlist[bd->num - 1], pos);
				undo(bd);
				return true;
			}

			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}

	// opp has dead4 and me has no four so me must defend
	// me has only one choice in this case
	if(pattern_read(pat(bd), DEAD4, opp))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if(pattern_read(hpinc(bd), DEAD4, opp) < 0)
			{
				mvlist_insert_front(&bd->hlist[bd->num - 1], pos);
				undo(bd);
				return true;
			}
			
			undo(bd);
			pos = mvlist_next(mlist(bd), pos);
		}
	}
	
	// opp has free3 and both have no four so me must defend
	if(pattern_read(pat(bd), FREE3, opp) || pattern_read(pat(bd), FREE3a, opp))
	{
		pos = mvlist_first(mlist(bd));
		while(pos != END)
		{
			do_move_no_mvlist(bd, pos, me);

			if(pattern_read(hpinc(bd), FREE4, me) > 0)
				mvlist_insert_front(&bd->hlist[bd->num - 1], pos);

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
	pair_t pair[15 * 15];
	u8 pos, i, cnt = 0;

	mvlist_remove_all(hlist(bd));

	if(must_do_generate(bd, me, opp))
		return;

	if(bd->num == 0)
		mvlist_insert_front(hlist(bd), 112);

	// generate helper array
	pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		do_move_no_mvlist(bd, pos, me);
		pair[cnt].pos = pos;
		pair[cnt++].key = evaluate(bd, &srh->sc, me);
		undo(bd);
		pos = mvlist_next(mlist(bd), pos);
	}
	
	// sort pair_t array descending
	pair_sort(pair, cnt);
	if(cnt < srh->leaf)
	{
		for(i = 0; i < cnt; i++)
			mvlist_insert_back(hlist(bd), pair[i].pos);
	}
	else
	{
		for(i = 0; i < srh->leaf; i++)
			mvlist_insert_back(hlist(bd), pair[i].pos);
	}
}

static void heuristic_generate_root(board_t* bd, const search_t* srh,
							const u8 dep, const u8 me, const u8 opp)
{
	pair_t pair[15 * 15];
	u8 pos, i, cnt = 0;

	mvlist_remove_all(hlist(bd));

	if(must_do_generate(bd, me, opp))
		return;

	if(bd->num == 0)
		mvlist_insert_front(hlist(bd), 112);

	// generate helper array
	pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		do_move(bd, pos, me);
		pair[cnt].pos = pos;
		// here different from the former function
		pair[cnt++].key = alphabeta(bd, srh, dep - 1, opp, LOSE - 1, WIN + 1, &i, 1);
		undo(bd);
		pos = mvlist_next(mlist(bd), pos);
	}
	
	// sort pair_t array descending
	pair_sort(pair, cnt);
	if(cnt < srh->leaf)
	{
		for(i = 0; i < cnt; i++)
			mvlist_insert_back(hlist(bd), pair[i].pos);
	}
	else
	{
		for(i = 0; i < srh->leaf; i++)
			mvlist_insert_back(hlist(bd), pair[i].pos);
	}
}

long alphabeta(board_t* bd, const search_t* srh, const u8 dep, 
				const u8 next, long alpha, long beta, u8* best, const bool heu)
{
	long val;
	u8 pos, tmp;
	tmp = board_gameover(bd);

	if(tmp == srh->me)
		return srh->sc.win - bd->num;
	if(tmp == srh->opp)
		return srh->sc.lose + bd->num;
	if(tmp == DRAW)
		return 0;
	if(dep <= 0)
		return evaluate(bd, &srh->sc, srh->me);

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
				do_move_no_mvlist(bd, pos, srh->opp);

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
				do_move_no_mvlist(bd, pos, srh->me);

			val = alphabeta(bd, srh, dep - 1, srh->opp,	alpha, beta, &tmp, 1);
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
	return 0;
}

u8 heuristic(board_t* bd, const search_t* srh)
{
	u8 tmp = 0;
	
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

	if(srh->dep >= 6 && srh->presrh)
	{
		heuristic_generate_root(bd, srh, srh->dep - 4, srh->me, srh->opp);
		alphabeta(bd, srh, srh->dep, srh->me, LOSE - 1, WIN + 1, &tmp, 0);
	}
	else
	{
		alphabeta(bd, srh, srh->dep, srh->me, LOSE - 1, WIN + 1, &tmp, 1);
	}

	return tmp;
}

