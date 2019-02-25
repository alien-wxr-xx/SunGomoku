/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * book.h - opening book functions
 */

#ifndef __BOOK_H__
#define __BOOK_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"
#include "board.h"

/*
 * Initialize optree from a randomly chosen opening book.
 */
void book_choose_direct();
void book_choose_indirect();

/*
 * Return true if book is loaded. Or return false.
 */
bool book_isload();

/*
 * Reset optree.
 */
void book_reset();

/*
 * Delete optree.
 */
void book_delete();

/*
 * Generate hlist using opening book.
 * Return true if find moves in the book. Or return false.
 */
bool book_generate(board_t* bd);

#ifdef  __cplusplus
}
#endif

#endif

