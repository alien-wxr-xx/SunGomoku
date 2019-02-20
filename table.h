/*                     _______
 *  Gomoku software   / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * table.h - lookup table generation
 */

#ifndef __TABLE_H__
#define __TABLE_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"
#include "pattern.h"
#include "mvlist.h"

// table element size
#define NEI_SIZE	16
#define PATTAB_NUM	4

// pattern lookup operation
#define INC			0
#define DEC			1

// pattern type macros
#define B6			0
#define B5			1
#define Bf4			2
#define Bd4			3
#define Bf3a		4
#define Bf3			5
#define Bd3			6
#define Bf2a		7
#define Bf2			8
#define Bd2			9
#define Bf1a		10
#define Bf1			11
#define Bd1			12

#define W6			13
#define W5			14
#define Wf4			15
#define Wd4			16
#define Wf3a		17
#define Wf3			18
#define Wd3			19
#define Wf2a		20
#define Wf2			21
#define Wd2			22
#define Wf1a		23
#define Wf1			24
#define Wd1			25

// lookup tables
extern u8 nei[15 * 15][NEI_SIZE];
extern u8 pattab15[P15][PATTAB_NUM];
extern u8 pattab14[P14][PATTAB_NUM];
extern u8 pattab13[P13][PATTAB_NUM];
extern u8 pattab12[P12][PATTAB_NUM];
extern u8 pattab11[P11][PATTAB_NUM];
extern u8 pattab10[P10][PATTAB_NUM];
extern u8 pattab9[P9][PATTAB_NUM];
extern u8 pattab8[P8][PATTAB_NUM];
extern u8 pattab7[P7][PATTAB_NUM];
extern u8 pattab6[P6][PATTAB_NUM];
extern u8 pattab5[P5][PATTAB_NUM];

/*
 * Lookup pattern.
 *
 * @param[out] pat		Output pattern_t structure.
 * @param[in]  index	Line index.
 * @param[in]  len		Line length.
 * @param[in]  op		Operation for pat.
 */
void line_pattern_lookup(pattern_t* pat, const u32 index, const u8 len, const u8 op);

/*
 * Generate neighbor lookup table.
 */
void nei_table_init();

/*
 * Generate pattern lookup tables.
 */
void pattern_table_init1();
void pattern_table_init2();

#ifdef  __cplusplus
}
#endif

#endif

