/*                     _______
 *  Gomoku Engine     / _____/
 *                   / /______  ________
 *  developed by    /____  / / / / __  /
 *                 _____/ / /_/ / / / /
 *  2019.1        /______/_____/_/ /_/
 *
 * tree.c - opening book tree implementation
 */

#include "tree.h" 
#include "macro.h"
#include "key.h"
#include "mvlist.h"
#include "board.h"

// #include "interface.h"

tree_t* tree_init()
{
	tree_t* tree = (tree_t*)malloc(sizeof(tree_t));
	tnode_t* root = (tnode_t*)malloc(sizeof(tnode_t));

	tree->root = root;
	tree->tptr = root;
	tree->list[0] = 112;
	tree->num = 1;

	// set root H8 and MID_RIGHT
	tree->root->pos = 112;
	tree->root->info = MID_RIGHT;
	tree->root->up = NULL;
	tree->root->down = NULL;
	tree->root->next = NULL;
	key_gen_arr(&root->key, tree->list, tree->num);

	return tree;
}

void tree_insert(tree_t* tree, const u8 pos, const u8 info)
{
	tnode_t* node = (tnode_t*)malloc(sizeof(tnode_t));

	node->pos = pos;
	node->info = info;
	node->up = NULL;
	node->down = NULL;
	node->next = NULL;

	// info 0x00 or 0x80
	if(tree->tptr->info == MID || tree->tptr->info == MID_RIGHT)
	{
		tree->tptr->down = node;
		node->up = tree->tptr;
	}

	// info 0xc0
	else if(tree->tptr->info == LEAF)
	{
		tree->num--;
		node->up = tree->tptr->up;
		tree->tptr->next = node;
	}

	// info 0x40
	else if(tree->tptr->info == LEAF_RIGHT)
	{
		tree->num--;
		while(tree->tptr->info != MID)
		{
			tree->num--;
			tree->tptr = tree->tptr->up;
		}
		node->up = tree->tptr->up;
		tree->tptr->next = node;
	}

	tree->list[tree->num++] = pos;
	key_gen_arr(&node->key, tree->list, tree->num);
	tree->tptr = node;
#if 0
	for(int i = 0; i < tree->num; i++)
	{
		print_position(tree->list[i]);
		printf(" ");
	}
	putchar('\n');
#endif
}

static void tree_reset_helper(tree_t* tree)
{
	tree_t sub;
	tnode_t* tmp;
	
	if(tree->root->down == NULL)
		return;
	else
	{
		sub.root = tree->root->down;
		while(1)
		{
			tree_reset(&sub);
			tmp = sub.root->next;
			free(sub.root);
			sub.root = tmp;
			if(sub.root == NULL)
				break;
		}
	}
	
	tree->root->down = NULL;
}

void tree_reset(tree_t* tree)
{
	tree_reset_helper(tree);
	tree->tptr = tree->root;
	tree->num = 1;
}

void tree_delete(tree_t* tree)
{
	tree_reset(tree);
	free(tree->root);
	free(tree);
}

void preorder_traversal(const tree_t* tree)
{
	tree_t sub;

#if 0
	printf("%d\t\t%x\n", tree->root->pos, tree->root->info);
#endif
#if 0
	print_position(tree->root->pos);
	printf("\t\t%x\t\t", tree->root->info);
	if(tree->root->up != NULL)
	{
		print_position(tree->root->up->pos);
		printf("\t\t");
	}
	else
		printf("%d\t\t", tree->root->up);
	if(tree->root->down != NULL)
	{
		print_position(tree->root->down->pos);
		printf("\t\t");
	}
	else
		printf("%d\t\t", tree->root->down);
	if(tree->root->next != NULL)
	{
		print_position(tree->root->next->pos);
		printf("\n");
	}
	else
		printf("%d\n", tree->root->next);
#endif

	if(tree->root->down == NULL)
		return;
	else
	{
		sub.root = tree->root->down;
		while(1)
		{
			preorder_traversal(&sub);
			sub.root = sub.root->next;
			if(sub.root == NULL)
				break;
		}
	}
}

bool tree_find_key(const tree_t* tree, const key_t* key, const int dep, board_t* bd)
{
	tree_t sub;
	tnode_t* tmp;

	if(key_issame(&tree->root->key, key))
	{
		tmp = tree->root->down;
		while(tmp != NULL)
		{
			mvlist_insert_back(hlist(bd), tmp->pos);
			tmp = tmp->next;
		}
		return true;
	}

	if(dep <= 0)
		return false;

	if(tree->root->down == NULL)
		return false;
	else
	{
		sub.root = tree->root->down;
		while(1)
		{
			if(tree_find_key(&sub, key, dep - 1, bd))
				return true;
			sub.root = sub.root->next;
			if(sub.root == NULL)
				break;
		}
	}
	return false;
}

