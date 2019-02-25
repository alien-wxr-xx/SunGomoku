/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * book.c - opening book functions
 */

#include "book.h"
#include "macro.h"
#include "tree.h"
#include "board.h"

// #include "interface.h"

#define DIRECT_NUM		6
#define INDIRECT_NUM	5
#define TRANS_NUM		8
#define MAX_DFS_DEP		13

/*******************************************************************************
							Tree generation functions
*******************************************************************************/
// direct opening file directory
static char* opening_d[DIRECT_NUM] = {
	"opening/d4-2.lib",
	"opening/d5-1.lib",
	"opening/d8-1.lib",
	"opening/d10-1.lib",
	"opening/d11-1.lib",
	"opening/d12-1.lib",
};

// indirect opening file directory
static char* opening_id[INDIRECT_NUM] = {
	"opening/id4-2.lib",
	"opening/id7-2.lib",
	"opening/id9-1.lib",
	"opening/id10-2.lib",
	"opening/id12-1.lib",
};

#if 0
// direct opening file header length in byte
static int header_d[DIRECT_NUM] = {
	22,
	44,
	22,
	22,
	24,
	22
};

// direct opening file header length in byte
static int header_id[INDIRECT_NUM] = {
	22,
	22,
	44,
	22,
	24,
	22
};
#endif

// opening tree pointer
static tree_t* optree = NULL;

bool book_isload()
{
	if(optree != NULL && optree->root->down != NULL)
		return true;
	else
		return false;
}

void book_reset()
{
	if(optree->root != NULL)
		tree_reset(optree);
}

void book_delete()
{
	if(optree != NULL)
		tree_delete(optree);
}

/*
 * Scan an opening file and insert nodes to optree.
 * @param [in]	The global variable opening tree.
 * @param [in]	The directory of the opening file.	
 * Return false if fails.
 * Notice: Only accept lib file without any comment.
 */
static bool book_load(tree_t* tree, char* dir)
{
	FILE* fin;
	u8 buf[2], pos, info, i;
	u32 cnt = 1;
	
	if((fin = fopen(dir, "rb")) == NULL)
	{
		printf("can't open book file!\n");
		return false;
	}

	// discard header and 78 xx.
	if(dir[9] == '5')
	{
		for(i = 0; i < 44; i++)
			if(fread(buf, 1, 1, fin));
	}
	else
	{
		while(fread(buf, 1, 1, fin) == 1)
		{
			if(buf[0] == 0x78)
			{
				if(fread(buf, 1, 1, fin));
				break;
			}
		}
	}

	// starts from the second move
	while(fread(buf, 1, 2, fin) == 2)
	{
		cnt++;
		pos = (buf[0] / 16) * 15 + buf[0] % 16 - 1;
		info = buf[1];

		// set second move and third move node info
		if(cnt == 2 || cnt == 3)
			info = MID;

		if(info != MID && info != MID_RIGHT && info != LEAF && info != LEAF_RIGHT)
		{
			printf("node info error!\n");
			return false;
		}

		tree_insert(tree, pos, info);
#if 0
		printf("%d\t\t%x\n", pos, info);
#endif
	}

	fclose(fin);
	return true;
}

void book_choose_direct()
{
	int index = rand() % DIRECT_NUM;

	if(optree == NULL)
		optree = tree_init();
	else
		book_reset(optree);

	if(!book_load(optree, opening_d[index]))
		printf("failed to load books!\n");
#if 0
	putchar('\n');
	preorder_traversal(optree);
#endif
}

void book_choose_indirect()
{
	int index = rand() % INDIRECT_NUM;

	if(optree == NULL)
		optree = tree_init();
	else
		book_reset(optree);

	if(!book_load(optree, opening_id[index]))
		printf("failed to load books!\n");
#if 0
	putchar('\n');
	preorder_traversal(optree);
#endif
}

/*******************************************************************************
							Board Transfrom functions
*******************************************************************************/
static u8 cc_arr[255] = {
	14,  29,  44,  59,  74,  89, 104, 119, 134, 149, 164, 179, 194, 209, 224,
	13,  28,  43,  58,  73,  88, 103, 118, 133, 148, 163, 178, 193, 208, 223,
	12,  27,  42,  57,  72,  87, 102, 117, 132, 147, 162, 177, 192, 207, 222,
	11,  26,  41,  56,  71,  86, 101, 116, 131, 146, 161, 176, 191, 206, 221,
	10,  25,  40,  55,  70,  85, 100, 115, 130, 145, 160, 175, 190, 205, 220,
	 9,  24,  39,  54,  69,  84,  99, 114, 129, 144, 159, 174, 189, 204, 219,
	 8,  23,  38,  53,  68,  83,  98, 113, 128, 143, 158, 173, 188, 203, 218,
	 7,  22,  37,  52,  67,  82,  97, 112, 127, 142, 157, 172, 187, 202, 217,
	 6,  21,  36,  51,  66,  81,  96, 111, 126, 141, 156, 171, 186, 201, 216,
	 5,  20,  35,  50,  65,  80,  95, 110, 125, 140, 155, 170, 185, 200, 215,
	 4,  19,  34,  49,  64,  79,  94, 109, 124, 139, 154, 169, 184, 199, 214,
	 3,  18,  33,  48,  63,  78,  93, 108, 123, 138, 153, 168, 183, 198, 213,
	 2,  17,  32,  47,  62,  77,  92, 107, 122, 137, 152, 167, 182, 197, 212,
	 1,  16,  31,  46,  61,  76,  91, 106, 121, 136, 151, 166, 181, 196, 211,
	 0,  15,  30,  45,  60,  75,  90, 105, 120, 135, 150, 165, 180, 195, 210
};

static u8 ref_arr[255] =
{
	 14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2,   1,   0,
	 29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,
	 44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,  30,
	 59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47,  46,  45,
	 74,  73,  72,  71,  70,  69,  68,  67,  66,  65,  64,  63,  62,  61,  60,
	 89,  88,  87,  86,  85,  84,  83,  82,  81,  80,  79,  78,  77,  76,  75,
	 104, 103, 102, 101, 100, 99,  98,  97,  96,  95,  94,  93,  92,  91,  90,
	 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105,
	 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120,
	 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135,
	 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150,
	 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165,
	 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180,
	 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195,
	 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210
};

static void cc(board_t* bd)
{
	u8 arr[15* 15];
	u8 pos, i;
	mvlist_t tmp;
	mvlist_reset(&tmp);

	// array
	for(i = 0; i < 15 * 15; i++)
		arr[i] = bd->arr[cc_arr[i]];
	for(i = 0; i < 15 * 15; i++)
		bd->arr[i] = arr[i];

	// mstk
	mvlist_remove_all(&tmp);
	pos = mvlist_first(mstk(bd));
	while(pos != END)
	{
		mvlist_insert_back(&tmp, cc_arr[pos]);
		pos = mvlist_next(mstk(bd), pos);
	}
	mvlist_copy(&tmp, mstk(bd));

	// mlist
	mvlist_remove_all(&tmp);
	pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		mvlist_insert_back(&tmp, cc_arr[pos]);
		pos = mvlist_next(mlist(bd), pos);
	}
	mvlist_copy(&tmp, mlist(bd));

	// hlist
	mvlist_remove_all(&tmp);
	pos = mvlist_first(hlist(bd));
	while(pos != END)
	{
		mvlist_insert_back(&tmp, cc_arr[pos]);
		pos = mvlist_next(hlist(bd), pos);
	}
	mvlist_copy(&tmp, hlist(bd));
}

static void ref(board_t* bd)
{
	u8 arr[15 * 15];
	u8 pos, i;
	mvlist_t tmp;
	mvlist_reset(&tmp);

	// array
	for(i = 0; i < 15 * 15; i++)
		arr[i] = bd->arr[ref_arr[i]];
	for(i = 0; i < 15 * 15; i++)
		bd->arr[i] = arr[i];

	// mstk
	mvlist_remove_all(&tmp);
	pos = mvlist_first(mstk(bd));
	while(pos != END)
	{
		mvlist_insert_back(&tmp, ref_arr[pos]);
		pos = mvlist_next(mstk(bd), pos);
	}
	mvlist_copy(&tmp, mstk(bd));

	// mlist
	mvlist_remove_all(&tmp);
	pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		mvlist_insert_back(&tmp, ref_arr[pos]);
		pos = mvlist_next(mlist(bd), pos);
	}
	mvlist_copy(&tmp, mlist(bd));

	// hlist
	mvlist_remove_all(&tmp);
	pos = mvlist_first(hlist(bd));
	while(pos != END)
	{
		mvlist_insert_back(&tmp, ref_arr[pos]);
		pos = mvlist_next(hlist(bd), pos);
	}
	mvlist_copy(&tmp, hlist(bd));
}

static void r0(board_t* bd)
{
	;
}

static void r0x(board_t* bd)
{
	ref(bd);
}

static void r90(board_t* bd)
{
	cc(bd);
}

static void r90x(board_t* bd)
{
	cc(bd);
	ref(bd);
}

static void r90xx(board_t* bd)
{
	ref(bd);
	cc(bd);
}

static void r180(board_t* bd)
{
	cc(bd);
	cc(bd);
}

static void r180x(board_t* bd)
{
	cc(bd);
	cc(bd);
	ref(bd);
}

static void r180xx(board_t* bd)
{
	ref(bd);
	cc(bd);
	cc(bd);
}

static void r270(board_t* bd)
{
	cc(bd);
	cc(bd);
	cc(bd);
}

static void r270x(board_t* bd)
{
	cc(bd);
	cc(bd);
	cc(bd);
	ref(bd);
}

static void r270xx(board_t* bd)
{
	ref(bd);
	cc(bd);
	cc(bd);
	cc(bd);
}

static void (*trans[TRANS_NUM])(board_t* bd) = {
	r0, r0x, r90, r90x, r180, r180x, r270, r270x
};

static void (*ctrans[TRANS_NUM])(board_t* bd) = {
	r0, r0x, r270, r270xx, r180, r180xx, r90, r90xx
};

/*******************************************************************************
								Tree Search functions
*******************************************************************************/
// variable saving the former board transform index
static int TransIndex = 0;

/*
 * Preorder traverse the optree for the current mvlist key. Generate hlist(bd).
 * Cannot solve permutation situation with the undown move.
 */
static void book_generate_dfs(board_t* bd)
{
	key_t key;
	mvlist_remove_all(hlist(bd));
	key_gen_mvlist(&key, mstk(bd));
	tree_find_key(optree, &key, bd->num, bd);
}

/*
 * Preorder traverse the optree for every possible mvlist key. Generate hlist(bd).
 * Can solve permutation situation with the undone move.
 */
static void book_generate_permutation(board_t* bd)
{
	key_t key;
	mvlist_t tmplist;
	mvlist_reset(&tmplist);

	u8 pos = mvlist_first(mlist(bd));
	while(pos != END)
	{
		mvlist_insert_back(mstk(bd), pos);
		key_gen_mvlist(&key, mstk(bd));
		mvlist_remove_back(mstk(bd));

		if(tree_find_key(optree, &key, bd->num, bd))
			mvlist_insert_back(&tmplist, pos);

		pos = mvlist_next(mlist(bd), pos);
	}

	mvlist_remove_all(hlist(bd));
	mvlist_copy(&tmplist, hlist(bd));
}

/*
 * Preorder traverse the optree. Generate hlist(bd).
 * Rotation and reflection are not considered.
 */
static void book_generate_no_trans(board_t* bd)
{
	book_generate_dfs(bd);
	if(!mvlist_size(hlist(bd)) && bd->num <= MAX_DFS_DEP)
		book_generate_permutation(bd);
}

bool book_generate(board_t* bd)
{
	int i;

	(*trans[TransIndex])(bd);
	book_generate_no_trans(bd);
	(*ctrans[TransIndex])(bd);

	if(!mvlist_size(hlist(bd)))
	{
		for(i = 0; i < TRANS_NUM; i++)
		{
			(*trans[i])(bd);
			book_generate_no_trans(bd);
			(*ctrans[i])(bd);

			if(mvlist_size(hlist(bd)))
			{
				TransIndex = i;
				break;
			}
		}
	}

	if(mvlist_size(hlist(bd)))
		return true;
	else
		return false;
}

