/*                     _______
 *  Gomoku software   / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * board.c - board_t data structure
 */

#include "board.h"
#include "macro.h"
#include "pattern.h"
#include "mvlist.h"
#include "table.h"

// global variable controlling if consider forbidden points
bool isForbidden = true;

/*******************************************************************************
							Functions calculating index
*******************************************************************************/
// macros calculating table index
#define IND15(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)	( \
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11 +	\
	bd->arr[a13]*P12 + bd->arr[a14]*P13 + bd->arr[a15]*P14						\
)

#define IND14(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)	( 	\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11 +	\
	bd->arr[a13]*P12 + bd->arr[a14]*P13											\
)

#define IND13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)	( 		\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11 +	\
	bd->arr[a13]*P12															\
)

#define IND12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)	( 			\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10 + bd->arr[a12]*P11		\
)

#define IND11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)	( 					\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9 + bd->arr[a11]*P10							\
)

#define IND10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)	( 						\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8 + bd->arr[a10]*P9											\
)

#define IND9(a1, a2, a3, a4, a5, a6, a7, a8, a9)	( 							\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7 +			\
	bd->arr[a9]*P8																\
)

#define IND8(a1, a2, a3, a4, a5, a6, a7, a8)	( 								\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6 + bd->arr[a8]*P7			\
)

#define IND7(a1, a2, a3, a4, a5, a6, a7)	( 									\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5 + bd->arr[a7]*P6							\
)

#define IND6(a1, a2, a3, a4, a5, a6)	( 										\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4 + bd->arr[a6]*P5												\
)

#define IND5(a1, a2, a3, a4, a5)	( 											\
	bd->arr[a1]*P0 + bd->arr[a2]*P1 + bd->arr[a3]*P2 + bd->arr[a4]*P3 +			\
	bd->arr[a5]*P4																\
)

// return row indices
inline static u32 r1(board_t* bd)
{
	return IND15(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
}

inline static u32 r2(board_t* bd)
{
	return IND15(15,16,17,18,19,20,21,22,23,24,25,26,27,28,29);
}
inline static u32 r3(board_t* bd)
{
	return IND15(30,31,32,33,34,35,36,37,38,39,40,41,42,43,44);
}

inline static u32 r4(board_t* bd)
{
	return IND15(45,46,47,48,49,50,51,52,53,54,55,56,57,58,59);
}

inline static u32 r5(board_t* bd)
{
	return IND15(60,61,62,63,64,65,66,67,68,69,70,71,72,73,74);
}

inline static u32 r6(board_t* bd)
{
	return IND15(75,76,77,78,79,80,81,82,83,84,85,86,87,88,89);
}

inline static u32 r7(board_t* bd)
{
	return IND15(90,91,92,93,94,95,96,97,98,99,100,101,102,103,104);
}

inline static u32 r8(board_t* bd)
{
	return IND15(105,106,107,108,109,110,111,112,113,114,115,116,117,118,119);
}

inline static u32 r9(board_t* bd)
{
	return IND15(120,121,122,123,124,125,126,127,128,129,130,131,132,133,134);
}

inline static u32 r10(board_t* bd)
{
	return IND15(135,136,137,138,139,140,141,142,143,144,145,146,147,148,149);
}

inline static u32 r11(board_t* bd)
{
	return IND15(150,151,152,153,154,155,156,157,158,159,160,161,162,163,164);
}

inline static u32 r12(board_t* bd)
{
	return IND15(165,166,167,168,169,170,171,172,173,174,175,176,177,178,179);
}

inline static u32 r13(board_t* bd)
{
	return IND15(180,181,182,183,184,185,186,187,188,189,190,191,192,193,194);
}

inline static u32 r14(board_t* bd)
{
	return IND15(195,196,197,198,199,200,201,202,203,204,205,206,207,208,209);
}

inline static u32 r15(board_t* bd)
{
	return IND15(210,211,212,213,214,215,216,217,218,219,220,221,222,223,224);
}

// return column indices
inline static u32 c1(board_t* bd)
{
	return IND15(0,15,30,45,60,75,90,105,120,135,150,165,180,195,210);
}

inline static u32 c2(board_t* bd)
{
	return IND15(1,16,31,46,61,76,91,106,121,136,151,166,181,196,211);
}

inline static u32 c3(board_t* bd)
{
	return IND15(2,17,32,47,62,77,92,107,122,137,152,167,182,197,212);
}

inline static u32 c4(board_t* bd)
{
	return IND15(3,18,33,48,63,78,93,108,123,138,153,168,183,198,213);
}

inline static u32 c5(board_t* bd)
{
	return IND15(4,19,34,49,64,79,94,109,124,139,154,169,184,199,214);
}

inline static u32 c6(board_t* bd)
{
	return IND15(5,20,35,50,65,80,95,110,125,140,155,170,185,200,215);
}

inline static u32 c7(board_t* bd)
{
	return IND15(6,21,36,51,66,81,96,111,126,141,156,171,186,201,216);
}

inline static u32 c8(board_t* bd)
{
	return IND15(7,22,37,52,67,82,97,112,127,142,157,172,187,202,217);
}

inline static u32 c9(board_t* bd)
{
	return IND15(8,23,38,53,68,83,98,113,128,143,158,173,188,203,218);
}

inline static u32 c10(board_t* bd)
{
	return IND15(9,24,39,54,69,84,99,114,129,144,159,174,189,204,219);
}

inline static u32 c11(board_t* bd)
{
	return IND15(10,25,40,55,70,85,100,115,130,145,160,175,190,205,220);
}

inline static u32 c12(board_t* bd)
{
	return IND15(11,26,41,56,71,86,101,116,131,146,161,176,191,206,221);
}

inline static u32 c13(board_t* bd)
{
	return IND15(12,27,42,57,72,87,102,117,132,147,162,177,192,207,222);
}

inline static u32 c14(board_t* bd)
{
	return IND15(13,28,43,58,73,88,103,118,133,148,163,178,193,208,223);
}

inline static u32 c15(board_t* bd)
{
	return IND15(14,29,44,59,74,89,104,119,134,149,164,179,194,209,224);
}

// return main diagoanl indices
// md1 -> md29 corresponds to upper right -> lower left main diagonals
inline static u32 md1(board_t* bd)
{
	return 0;
}

inline static u32 md2(board_t* bd)
{
	return 0;
}

inline static u32 md3(board_t* bd)
{
	return 0;
}

inline static u32 md4(board_t* bd)
{
	return 0;
}

inline static u32 md5(board_t* bd)
{
	return IND5(10,26,42,58,74);
}

inline static u32 md6(board_t* bd)
{
	return IND6(9,25,41,57,73,89);
}

inline static u32 md7(board_t* bd)
{
	return IND7(8,24,40,56,72,88,104);
}

inline static u32 md8(board_t* bd)
{
	return IND8(7,23,39,55,71,87,103,119);
}

inline static u32 md9(board_t* bd)
{
	return IND9(6,22,38,54,70,86,102,118,134);
}

inline static u32 md10(board_t* bd)
{
	return IND10(5,21,37,53,69,85,101,117,133,149);
}

inline static u32 md11(board_t* bd)
{
	return IND11(4,20,36,52,68,84,100,116,132,148,164);
}

inline static u32 md12(board_t* bd)
{
	return IND12(3,19,35,51,67,83,99,115,131,147,163,179);
}

inline static u32 md13(board_t* bd)
{
	return IND13(2,18,34,50,66,82,98,114,130,146,162,178,194);
}

inline static u32 md14(board_t* bd)
{
	return IND14(1,17,33,49,65,81,97,113,129,145,161,177,193,209);
}

inline static u32 md15(board_t* bd)
{
	return IND15(0,16,32,48,64,80,96,112,128,144,160,176,192,208,224);
}

inline static u32 md16(board_t* bd)
{
	return IND14(15,31,47,63,79,95,111,127,143,159,175,191,207,223);
}

inline static u32 md17(board_t* bd)
{
	return IND13(30,46,62,78,94,110,126,142,158,174,190,206,222);
}

inline static u32 md18(board_t* bd)
{
	return IND12(45,61,77,93,109,125,141,157,173,189,205,221);
}

inline static u32 md19(board_t* bd)
{
	return IND11(60,76,92,108,124,140,156,172,188,204,220);
}

inline static u32 md20(board_t* bd)
{
	return IND10(75,91,107,123,139,155,171,187,203,219);
}

inline static u32 md21(board_t* bd)
{
	return IND9(90,106,122,138,154,170,186,202,218);
}

inline static u32 md22(board_t* bd)
{
	return IND8(105,121,137,153,169,185,201,217);
}

inline static u32 md23(board_t* bd)
{
	return IND7(120,136,152,168,184,200,216);
}

inline static u32 md24(board_t* bd)
{
	return IND6(135,151,167,183,199,215);
}

inline static u32 md25(board_t* bd)
{
	return IND5(150,166,182,198,214);
}

inline static u32 md26(board_t* bd)
{
	return 0;
}

inline static u32 md27(board_t* bd)
{
	return 0;
}

inline static u32 md28(board_t* bd)
{
	return 0;
}

inline static u32 md29(board_t* bd)
{
	return 0;
}

// return anti-diagonal indices
// ad1 -> ad29 corresponds to upper left -> lower right anti-diagonals
inline static u32 ad1(board_t* bd)
{
	return 0;
}

inline static u32 ad2(board_t* bd)
{
	return 0;
}

inline static u32 ad3(board_t* bd)
{
	return 0;
}

inline static u32 ad4(board_t* bd)
{
	return 0;
}

inline static u32 ad5(board_t* bd)
{
	return IND5(4,18,32,46,60);
}

inline static u32 ad6(board_t* bd)
{
	return IND6(5,19,33,47,61,75);
}

inline static u32 ad7(board_t* bd)
{
	return IND7(6,20,34,48,62,76,90);
}

inline static u32 ad8(board_t* bd)
{
	return IND8(7,21,35,49,63,77,91,105);
}

inline static u32 ad9(board_t* bd)
{
	return IND9(8,22,36,50,64,78,92,106,120);
}

inline static u32 ad10(board_t* bd)
{
	return IND10(9,23,37,51,65,79,93,107,121,135);
}

inline static u32 ad11(board_t* bd)
{
	return IND11(10,24,38,52,66,80,94,108,122,136,150);
}

inline static u32 ad12(board_t* bd)
{
	return IND12(11,25,39,53,67,81,95,109,123,137,151,165);
}

inline static u32 ad13(board_t* bd)
{
	return IND13(12,26,40,54,68,82,96,110,124,138,152,166,180);
}

inline static u32 ad14(board_t* bd)
{
	return IND14(13,27,41,55,69,83,97,111,125,139,153,167,181,195);
}

inline static u32 ad15(board_t* bd)
{
	return IND15(14,28,42,56,70,84,98,112,126,140,154,168,182,196,210);
}

inline static u32 ad16(board_t* bd)
{
	return IND14(29,43,57,71,85,99,113,127,141,155,169,183,197,211);
}

inline static u32 ad17(board_t* bd)
{
	return IND13(44,58,72,86,100,114,128,142,156,170,184,198,212);
}

inline static u32 ad18(board_t* bd)
{
	return IND12(59,73,87,101,115,129,143,157,171,185,199,213);
}

inline static u32 ad19(board_t* bd)
{
	return IND11(74,88,102,116,130,144,158,172,186,200,214);
}

inline static u32 ad20(board_t* bd)
{
	return IND10(89,103,117,131,145,159,173,187,201,215);
}

inline static u32 ad21(board_t* bd)
{
	return IND9(104,118,132,146,160,174,188,202,216);
}

inline static u32 ad22(board_t* bd)
{
	return IND8(119,133,147,161,175,189,203,217);
}

inline static u32 ad23(board_t* bd)
{
	return IND7(134,148,162,176,190,204,218);
}

inline static u32 ad24(board_t* bd)
{
	return IND6(149,163,177,191,205,219);
}

inline static u32 ad25(board_t* bd)
{
	return IND5(164,178,192,206,220);
}

inline static u32 ad26(board_t* bd)
{
	return 0;
}

inline static u32 ad27(board_t* bd)
{
	return 0;
}

inline static u32 ad28(board_t* bd)
{
	return 0;
}

inline static u32 ad29(board_t* bd)
{
	return 0;
}

static u32 (*r[15 * 15])(board_t* bd) = {
	r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,  r1,
	r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,  r2,
	r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,  r3,
	r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,  r4,
	r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,  r5,
	r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,  r6,
	r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,  r7,
	r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,  r8,
	r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,  r9,
	r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10, r10,
	r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11, r11,
	r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12, r12,
	r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13, r13,
	r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14, r14,
	r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15, r15
};

static u32 (*c[15 * 15])(board_t* bd) = {
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
	c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15
};

static u32 (*md[15 * 15])(board_t* bd) = {
	md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4, md3, md2, md1,
	md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4, md3, md2,
	md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4, md3,
	md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5, md4,
	md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6, md5,
	md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7, md6,
	md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8, md7,
	md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9, md8,
	md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,md9,
	md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,md10,
	md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,md11,
	md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,md12,
	md27,md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,md13,
	md28,md27,md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15,md14,
	md29,md28,md27,md26,md25,md24,md23,md22,md21,md20,md19,md18,md17,md16,md15
};

static u32 (*ad[15 * 15])(board_t* bd) = {
	ad1, ad2, ad3, ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,
	ad2, ad3, ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,
	ad3, ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,
	ad4, ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,
	ad5, ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,
	ad6, ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,
	ad7, ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,
	ad8, ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,
	ad9, ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,
	ad10,ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,
	ad11,ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,
	ad12,ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,
	ad13,ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,ad27,
	ad14,ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,ad27,ad28,
	ad15,ad16,ad17,ad18,ad19,ad20,ad21,ad22,ad23,ad24,ad25,ad26,ad27,ad28,ad29
};

static u8 md_len[15 * 15] = {
	15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,
	14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,
	13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,
	12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,
	11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,
	10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,
	9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,
	8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,
	7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,
	6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10,
	5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11,
	4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12,
	3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13,
	2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14,
	1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15
};

static u8 ad_len[15 * 15] = {
	1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 
	2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14,
	3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13,
	4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12,
	5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11,
	6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10,
	7,  8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,
	8,  9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,
	9,  10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,
	10, 11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,
	11, 12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,
	12, 13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,
	13, 14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,
	14, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,
	15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1
};

/*******************************************************************************
						board_t operation implementation
*******************************************************************************/
void board_reset(board_t* bd)
{
	int i;

	bd->num = 0;
	mvlist_reset(mstk(bd));
	pattern_reset(pinc(bd));
	pattern_reset(hpinc(bd));

	for(i = 0; i < 15 * 15; i++)
	{
		bd->arr[i] = EMPTY;
		pattern_reset(&bd->pat[i]);
		mvlist_reset(&bd->mlist[i]);
		mvlist_reset(&bd->hlist[i]);
	}
}

void board_init(board_t* bd, const char (*arr)[15])
{
	int r, c;
	board_reset(bd);
	for(r = 0; r < 15; r++)
		for(c = 0; c < 15; c++)
			do_move(bd, r * 15 + c, arr[r][c]);
}

u8 board_gameover(const board_t* bd)
{
	if(bd->num == 15 * 15)
		return DRAW;

	if(pattern_read(pat(bd), FIVE, BLACK))
		return BLACK;
	
	if(pattern_read(pat(bd), FIVE, WHITE))
		return WHITE;

	if(isForbidden)
	{
		if(pattern_read(pat(bd), LONG, BLACK) || pattern_read(pat(bd), LONG, WHITE))
			return WHITE;

		if((pattern_read(pinc(bd),FREE4,BLACK)+pattern_read(pinc(bd),DEAD4,BLACK)>1)
		|| (pattern_read(pinc(bd),FREE3,BLACK)+pattern_read(pinc(bd),FREE3a,BLACK)>1))
			return WHITE;
	}

	return false;
}

void do_move(board_t* bd, const u8 pos, const u8 color)
{
	if(bd->arr[pos] != EMPTY || color == EMPTY)
		return;
	
	bd->num++;

	// subtract old critical line patterns
	pattern_copy(&bd->pat[bd->num - 1], pat(bd));
	line_pattern_lookup(pat(bd), (*r[pos])(bd), 15, DEC);
	line_pattern_lookup(pat(bd), (*c[pos])(bd), 15, DEC);
	line_pattern_lookup(pat(bd), (*md[pos])(bd), md_len[pos], DEC);
	line_pattern_lookup(pat(bd), (*ad[pos])(bd), ad_len[pos], DEC);
	
	// make move
	bd->arr[pos] = color;
	mvlist_insert_back(mstk(bd), pos);

	// add new critical line patterns
	line_pattern_lookup(pat(bd), (*r[pos])(bd), 15, INC);
	line_pattern_lookup(pat(bd), (*c[pos])(bd), 15, INC);
	line_pattern_lookup(pat(bd), (*md[pos])(bd), md_len[pos], INC);
	line_pattern_lookup(pat(bd), (*ad[pos])(bd), ad_len[pos], INC);

	// update pinc
	pattern_sub(pinc(bd), pat(bd), &bd->pat[bd->num - 1]);

	// generate mvlist
	int i;
	mvlist_copy(&bd->mlist[bd->num - 1], mlist(bd));
	mvlist_remove(mlist(bd), pos);
	for(i = 0; i < NEI_SIZE; i++)
	{
		if(nei[pos][i] == INVALID)
			break;
		if(bd->arr[nei[pos][i]] == EMPTY)
		{
			mvlist_remove(mlist(bd), nei[pos][i]);
			mvlist_insert_front(mlist(bd), nei[pos][i]);
		}
	}
}

void do_move_no_mvlist(board_t* bd, const u8 pos, const u8 color)
{
	if(bd->arr[pos] != EMPTY || color == EMPTY)
		return;
	
	bd->num++;

	// subtract old critical line patterns
	pattern_copy(&bd->pat[bd->num - 1], pat(bd));
	line_pattern_lookup(pat(bd), (*r[pos])(bd), 15, DEC);
	line_pattern_lookup(pat(bd), (*c[pos])(bd), 15, DEC);
	line_pattern_lookup(pat(bd), (*md[pos])(bd), md_len[pos], DEC);
	line_pattern_lookup(pat(bd), (*ad[pos])(bd), ad_len[pos], DEC);
	
	// make move
	bd->arr[pos] = color;
	mvlist_insert_back(mstk(bd), pos);

	// add new critical line patterns
	line_pattern_lookup(pat(bd), (*r[pos])(bd), 15, INC);
	line_pattern_lookup(pat(bd), (*c[pos])(bd), 15, INC);
	line_pattern_lookup(pat(bd), (*md[pos])(bd), md_len[pos], INC);
	line_pattern_lookup(pat(bd), (*ad[pos])(bd), ad_len[pos], INC);

	// update hpinc
	pattern_sub(hpinc(bd), pat(bd), &bd->pat[bd->num - 1]);
}

void do_move_no_mvlist_pinc(board_t* bd, const u8 pos, const u8 color)
{
	if(bd->arr[pos] != EMPTY || color == EMPTY)
		return;
	
	bd->num++;

	// subtract old critical line patterns
	pattern_copy(&bd->pat[bd->num - 1], pat(bd));
	line_pattern_lookup(pat(bd), (*r[pos])(bd), 15, DEC);
	line_pattern_lookup(pat(bd), (*c[pos])(bd), 15, DEC);
	line_pattern_lookup(pat(bd), (*md[pos])(bd), md_len[pos], DEC);
	line_pattern_lookup(pat(bd), (*ad[pos])(bd), ad_len[pos], DEC);
	
	// make move
	bd->arr[pos] = color;
	mvlist_insert_back(mstk(bd), pos);

	// add new critical line patterns
	line_pattern_lookup(pat(bd), (*r[pos])(bd), 15, INC);
	line_pattern_lookup(pat(bd), (*c[pos])(bd), 15, INC);
	line_pattern_lookup(pat(bd), (*md[pos])(bd), md_len[pos], INC);
	line_pattern_lookup(pat(bd), (*ad[pos])(bd), ad_len[pos], INC);

	// update pinc
	pattern_sub(pinc(bd), pat(bd), &bd->pat[bd->num - 1]);
}

void undo(board_t* bd)
{
	if(bd->num == 0)
		return;
	else
	{
		bd->num--;
		bd->arr[mvlist_last(mstk(bd))] = EMPTY;
		mvlist_remove_back(mstk(bd));
	}
}

