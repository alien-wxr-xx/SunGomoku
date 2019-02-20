/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.2        /______/_____/_/ /_/
 *
 * tree.h - opening tree implementation
 *
 * Notice: the root node of this tree is H8.
 */

#ifndef __TREE_H__
#define __TREE_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include "macro.h"
#include "mvlist.h"
#include "key.h"
#include "board.h"

// node info macros
#define MID			0x80	// mid node with right and down nodes
#define MID_RIGHT	0x00	// rightmost node with down nodes
#define LEAF		0xc0	// leaf with right nodes
#define LEAF_RIGHT	0x40	// rightmost leaf

// tree node structure
typedef struct tnode {
	u8 pos;					// position
	u8 info;				// node info
	struct tnode* up;		// parent node
	struct tnode* down;		// children node
	struct tnode* next;		// brother node
	key_t key;				// path key
} tnode_t; 

// tree structure
typedef struct {
	tnode_t* root;		// root node
	tnode_t* tptr;		// pointer to the current node
	u8 list[15 * 15];	// move stack
	u8 num;				// list size
} tree_t;

/*
 * Return the pointer to a new tree.
 */
tree_t* tree_init();

/*
 * Insert a node.
 */
void tree_insert(tree_t* tree, const u8 pos, const u8 info);

/*
 * Reset a tree to root node.
 */
void tree_reset(tree_t* tree);

/*
 * Delete a tree.
 */
void tree_delete(tree_t* tree);

/*
 * Preorder print tree.
 */
void preorder_traversal(const tree_t* tree);

/*
 * Find key in the tree using depth first preorder traversal.
 * Add all children of the target nodes to hlist(bd).
 * Maximum search depth is limited to dep.
 */
bool tree_find_key(const tree_t* tree, const key_t* key, const int dep, board_t* bd);

#ifdef  __cplusplus
}
#endif

#endif

