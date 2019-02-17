/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * uiinc.c - interface functions for ui
 */

#include "uiinc.h"
#include "macro.h"
#include "board.h"
#include "search.h"
#include "book.h"

extern bool isForbidden;

static board_t Board;

search_t Srh = {
	.sc = {
		.win = WIN,
		.lose = LOSE,
		.free4 = 4320,
		.dead4 = 882,
		.free3 = 630,
		.dead3 = 294,
		.free2 = 210,
		.dead2 = 42,
		.free1 = 30,
		.dead1 = 1,
		.free3a = 693,
		.free2a = 231,
		.free1a = 33
	},
	.me = BLACK,
	.opp = WHITE,
	.leaf = 10,
	.dep = 10,
	.presrh = true,
	.book = true
};

void initialize()
{
	srand(time(0));
	nei_table_init();
	pattern_table_init1();
	pattern_table_init2();
}

void restart()
{
	board_reset(&Board);
	if(book_isload())
		book_reset();
}

void uninitialize()
{
	book_delete();
}

void set_forbidden(const int flag)
{
	if(flag)
		isForbidden = true;
	else
		isForbidden = false;
}

void set_difficulty(const int dif)
{
	if(isForbidden)
	{
		switch(dif)
		{
			case 0:
				Srh.dep = 4;
				Srh.book = false;
				break;
			case 1:
				Srh.dep = 8;
				Srh.book = false;
				break;
			case 2:
				Srh.dep = 10;
				Srh.book = true;
				break;
			default:
				break;
		}
	}
	else
	{
		switch(dif)
		{
			case 0:
				Srh.dep = 1;
				Srh.book = false;
				break;
			case 1:
				Srh.dep = 2;
				Srh.book = false;
				break;
			case 2:
				Srh.dep = 4;
				Srh.book = false;
				break;
			default:
				break;
		}
	}
}

void player_do_move(const int x, const int y, int* isover, const u8 color)
{
	do_move(&Board, x * 15 + y, color);
	*isover = board_gameover(&Board);
}

int ai_do_move(int* isover, const u8 color)
{
	int pos;

	if(color == BLACK)
	{
		Srh.me = BLACK;
		Srh.opp = WHITE;
	}
	else if(color == WHITE)
	{
		Srh.me = WHITE;
		Srh.opp = BLACK;
	}

	pos = heuristic(&Board, &Srh);
	do_move(&Board, pos, color);
	*isover = board_gameover(&Board);

	return pos;
}

void undo_move(const int N)
{
	int i;
	for(i = 0; i < N; i++)
		undo(&Board);
}

