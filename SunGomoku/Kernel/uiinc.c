/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
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
		.free4 = 5000,
		.dead4 = 882,
		.free3 = 630,
		.dead3 = 210,
		.free2 = 210,
		.dead2 = 25,
		.free1 = 25,
		.dead1 = 1,
		.free3a = 693,
		.free2a = 231,
		.free1a = 27
	},
	.hpleaf = 16,
	.htleaf = 10,
	.dep = 10,
	.tlimit = 3,
	.presrh = true,
	.book = false
};

void initialize()
{
	srand(time(0));
	nei_table_init();
//	pattern_table_init1();
//	pattern_table_init2();
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
    {
		isForbidden = true;
        pattern_table_init1();
        pattern_table_init2();
    }
	else
    {
		isForbidden = false;
        pattern_table_init1();
        pattern_table_init2();
    }
}

void set_difficulty(const int dif)
{
    switch(dif)
    {
        case 0:
            set_forbidden(false);
            Srh.dep = 10;
            Srh.book = false;
            break;
        case 1:
            set_forbidden(true);
            Srh.dep = 10;
            Srh.book = false;
            break;
        case 2:
            set_forbidden(true);
            Srh.dep = 10;
            Srh.book = true;
            break;
        default:
            break;
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

