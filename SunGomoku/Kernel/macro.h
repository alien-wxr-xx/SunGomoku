/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * macro.h - program macros
 */

#ifndef __MACRO_H__
#define __MACRO_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>

// options
#define SLOW_TABLE			1
#define TIME_INFO			1
#define LETTER_MOVE			1
#define DISPLAY_HISTORY		1
#define DISPLAY_BOARD		1
#define DISPLAY_PATTERN		0
#define DISPLAY_MVLIST		0
#define DISPLAY_HEU			0

// basic constants
#define BOARD_SIZE			15
#define EMPTY				0
#define BLACK				1
#define WHITE				2
#define DO					0
#define UNDO				1
#define DRAW				225
#define INVALID				255
#define WIN					100000
#define LOSE			   -100000

// Powers of three
#define P0			1
#define P1			3
#define P2			9
#define P3			27
#define P4			81
#define P5			243
#define P6			729
#define P7			2187
#define P8			6561
#define P9			19683
#define P10			59049
#define P11			177147
#define P12			531441
#define P13			1594323
#define P14			4782969
#define P15			14348907

// client macros
#define ME			1
#define OTHER		2
#define TRUE		1
#define FALSE		0
#define MAX_BYTE	10000

#define START		"START"
#define PLACE		"PLACE"
#define DONE		"DONE"
#define TURN		"TURN"
#define BEGIN		"BEGIN"
#define CEND		"END"

typedef	uint8_t		u8;
typedef	uint16_t	u16;
typedef	uint32_t	u32;
typedef	uint64_t	u64;

#if 1
#define JUDGE_END											\
	cnt++;													\
	if(board_gameover(&bd) == Srh1.me)						\
	{														\
		ai1_win += 1;										\
		if(Srh1.me == BLACK) printf("black wins\n");		\
		else if(Srh1.me == WHITE) printf("white wins\n");	\
		display_board(&bd);									\
		break;												\
	}														\
	else if(board_gameover(&bd) == Srh2.me)					\
	{														\
		ai2_win += 1;										\
		if(Srh2.me == BLACK) printf("black wins\n");		\
		else if(Srh2.me == WHITE) printf("white wins\n");	\
		display_board(&bd);									\
		break;												\
	}														\
	else if(board_gameover(&bd) == DRAW)					\
	{														\
		ai1_win += 0.5;										\
		ai2_win += 0.5;										\
		printf("draw\n");									\
		display_board(&bd);									\
		break;												\
	}
#endif

#if 0
#define JUDGE_END											\
	cnt++;													\
	if(board_gameover(&bd) == Srh1.me)						\
	{														\
		ai1_win += 1;										\
		break;												\
	}														\
	else if(board_gameover(&bd) == Srh2.me)					\
	{														\
		ai2_win += 1;										\
		break;												\
	}														\
	else if(board_gameover(&bd) == DRAW)					\
	{														\
		ai1_win += 0.5;										\
		ai2_win += 0.5;										\
		break;												\
	}
#endif

#ifdef  __cplusplus
}
#endif

#endif

