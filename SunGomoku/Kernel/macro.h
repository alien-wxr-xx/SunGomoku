/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
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

// display options
#define TIME_INFO			0
#define LETTER_MOVE			0
#define DISPLAY_HISTORY		0
#define DISPLAY_BOARD		0
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

typedef	uint8_t		u8;
typedef	uint16_t	u16;
typedef	uint32_t	u32;
typedef	uint64_t	u64;

#ifdef  __cplusplus
}
#endif

#endif

