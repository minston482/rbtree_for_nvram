#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree_group.h"
//#include "buddy_allocator.h"

struct simul_swap_node swap_head;
unsigned long long int swap_count = 0;

simul_rb_node *parent_of(simul_rb_node *node) {
	return node->rb_parent;
}

void simul_free(simul_rb_node *node) {
	free(node);
}

simul_rb_node *right_of(simul_rb_node *node) {
	return node->rb_right;
}

simul_rb_node *left_of(simul_rb_node *node) {
	return node->rb_left;
}

void init_swap_node() {
	swap_head.node = NULL;
	swap_head.prev = &swap_head;
	swap_head.next = &swap_head;
}

void init_rb_key(simul_rb_key *key_node, long long int key) {
	key_node->rb_key = key;
	init_list_head(&(key_node->list));
}

void init_rb_node(simul_rb_root *T, simul_rb_node *node, simul_rb_key *key_node) {
	set_rb_red(node);
	node->std_key = key_node->rb_key / GROUP_SIZE;
	init_list_head(&(node->rb_keys));
	list_add(&(key_node->list), &(node->rb_keys));
	node->rb_parent = T->nil_node;
	node->rb_right = T->nil_node;
	node->rb_left = T->nil_node;
	node->count = 1;
	node->is_free = 0;
}

void init_rb_root(simul_rb_root *root) {
	root->nil_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));
	root->root_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));

	set_rb_black(root->nil_node);
	root->nil_node->std_key = SIMUL_RB_NIL;
	init_list_head(&(root->nil_node->rb_keys));
	root->nil_node->rb_parent = root->nil_node;
	root->nil_node->rb_right = root->nil_node;
	root->nil_node->rb_left = root->nil_node;
	root->nil_node->count = 1;
	root->nil_node->is_free = 0;
	set_rb_black(root->root_node);
	root->root_node->std_key = SIMUL_RB_NIL;
	init_list_head(&(root->root_node->rb_keys));
	root->root_node->rb_parent = root->nil_node;
	root->root_node->rb_left = root->nil_node;
	root->root_node->rb_right = root->nil_node;
	root->root_node->count = 1;
	root->root_node->is_free = 0;
}

unsigned int is_rb_black(simul_rb_node *node) {
	return node->rb_color == SIMUL_RB_BLACK ? 1 : 0;
}

unsigned int is_rb_red(simul_rb_node *node) {
	return node->rb_color == SIMUL_RB_RED ? 1 : 0;
}

void set_rb_black(simul_rb_node *node) {
	node->rb_color = SIMUL_RB_BLACK;
}

void set_rb_red(simul_rb_node *node) {
	node->rb_color = SIMUL_RB_RED;
}

simul_rb_root *simul_rb_alloc_root(void) {
	simul_rb_root *ret = (simul_rb_root *)malloc(sizeof(simul_rb_root));
	init_rb_root(ret);
	return ret;
}

simul_rb_node *simul_rb_left_rotate(simul_rb_root *T, simul_rb_node *node) {
	simul_rb_node *ret = NULL;
	simul_rb_node *right_node = right_of(node);

	/* exchange left child of node_right to right child of node */
	node->rb_right = left_of(right_node);
	node->count++;   // for analysis
	//node = simul_rb_swapping(T, node);
	add_swap_list(node);

	if (left_of(right_node) != T->nil_node) {
		left_of(right_node)->rb_parent = node;
		left_of(right_node)->count++;   // for analysis
		//simul_rb_swapping(T, right_node->rb_left);
		add_swap_list(right_node->rb_left);
	}

	/* change parent connection */
	right_node->rb_parent = parent_of(node);
	right_node->count++;   // for analysis
	//right_node = simul_rb_swapping(T, right_node);
	add_swap_list(right_node);

	if (node == parent_of(node)->rb_left) {   /* case: node is left child of parent */
		parent_of(node)->rb_left = right_node;
		parent_of(node)->count++;   // for analysis
		//simul_rb_swapping(T, node->rb_parent);
		add_swap_list(node->rb_parent);
	}
	else if (node == parent_of(node)->rb_right) {   /* case: node is right child of parent */
		parent_of(node)->rb_right = right_node;
		parent_of(node)->count++;   // for analysis
		//simul_rb_swapping(T, node->rb_parent);
		add_swap_list(node->rb_parent);
	}

	right_node->rb_left = node;
	right_node->count++;   // for analysis
	//right_node = simul_rb_swapping(T, right_node);
	add_swap_list(right_node);
	node->rb_parent = right_node;
	node->count++;   // for analysis
	//node = simul_rb_swapping(T, node);
	add_swap_list(node);

	ret = node;

	return ret;
}

simul_rb_node *simul_rb_right_rotate(simul_rb_root *T, simul_rb_node *node) {
	simul_rb_node *ret = NULL;
	simul_rb_node *left_node = left_of(node);

	/* exchange right child of node_left to left child of node */
	node->rb_left = right_of(left_node);
	node->count++;   // for analysis
	//node = simul_rb_swapping(T, node);
	add_swap_list(node);

	if (right_of(left_node) != T->nil_node) {
		right_of(left_node)->rb_parent = node;
		right_of(left_node)->count++;   // for analysis
		//simul_rb_swapping(T, left_node->rb_right);
		add_swap_list(left_node->rb_right);
	}

	/* change parent connection */
	left_node->rb_parent = parent_of(node);
	left_node->count++;   // for analysis
	//left_node = simul_rb_swapping(T, left_node);
	add_swap_list(left_node);

	if (node == parent_of(node)->rb_left) {   /* case: node is left child of parent */
		parent_of(node)->rb_left = left_node;
		parent_of(node)->count++;   // for analysis
		//simul_rb_swapping(T, node->rb_parent);
		add_swap_list(node->rb_parent);
	}
	else if (node == parent_of(node)->rb_right) {   /* case: node is right child of parent */
		parent_of(node)->rb_right = left_node;
		parent_of(node)->count++;   // for analysis
		//simul_rb_swapping(T, node->rb_parent);
		add_swap_list(node->rb_parent);
	}

	left_node->rb_right = node;
	left_node->count++;   // for analysis
	//left_node = simul_rb_swapping(T, left_node);
	add_swap_list(left_node);
	node->rb_parent = left_node;
	node->count++;   // for analysis
	//node = simul_rb_swapping(T, node);
	add_swap_list(node);

	ret = node;

	return ret;
}

/* change parent of chg_node from parent of pre_node */
unsigned int simul_rb_transplant(simul_rb_root *T, simul_rb_node *pre_node, simul_rb_node *chg_node) {
	unsigned int ret = 0;

	if (parent_of(pre_node) == T->nil_node)   /* case: node is root node */
		T->root_node = chg_node;
	else if (pre_node == parent_of(pre_node)->rb_left)   /* case: node is left child of parent */
		parent_of(pre_node)->rb_left = chg_node;
	else if (pre_node == parent_of(pre_node)->rb_right)   /* case: node is right child of parent */
		parent_of(pre_node)->rb_right = chg_node;

	chg_node->rb_parent = parent_of(pre_node);   /* change chg_node's parent */

	return ret;
}

simul_rb_node *simul_rb_minimum(simul_rb_node *node) {
	simul_rb_node *min_node = node;
	while (left_of(min_node)->std_key != SIMUL_RB_NIL)
		min_node = left_of(min_node);

	return min_node;
}

simul_rb_node *simul_rb_successor(simul_rb_root* T, simul_rb_node *node) {
	simul_rb_node *y;
	simul_rb_node *nil = T->nil_node;
	simul_rb_node *root = T->root_node;

	if (nil != (y = node->rb_right)) {
		while (y->rb_left != nil) {
			y = y->rb_left;
		}
		return (y);
	}
	else {
		y = node->rb_parent;
		while (node == y->rb_right) {
			node = y;
			y = y->rb_parent;
		}
		if (y == root) return (nil);
		return (y);
	}
}

void simul_rb_swapping(simul_rb_root *T) {
	simul_rb_node *new_node = NULL;
	struct simul_swap_node *cur_swap_node = swap_head.next, *del_swap_node = NULL;

	while (cur_swap_node != &swap_head) {
		if (cur_swap_node->node->is_free == 0) {
			swap_count++;
			new_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));
			new_node->rb_color = cur_swap_node->node->rb_color;
			new_node->std_key = cur_swap_node->node->std_key;
			new_node->rb_keys.next = cur_swap_node->node->rb_keys.next;
			new_node->rb_keys.next->prev = &(new_node->rb_keys);
			new_node->rb_keys.prev = cur_swap_node->node->rb_keys.prev;
			new_node->rb_keys.prev->next = &(new_node->rb_keys);
			new_node->rb_parent = cur_swap_node->node->rb_parent;
			new_node->rb_left = cur_swap_node->node->rb_left;
			new_node->rb_right = cur_swap_node->node->rb_right;
			new_node->is_free = 0;
			new_node->count = 1;

			// parent pointer 
			if (cur_swap_node->node == T->root_node)
				T->root_node = new_node;
			else if (cur_swap_node->node->rb_parent == T->root_node) 	// old_node is root
				T->root_node->rb_left = new_node;
			else if (cur_swap_node->node->rb_parent->rb_left == cur_swap_node->node) 	// old_node is parent's left child
				cur_swap_node->node->rb_parent->rb_left = new_node;
			else if (cur_swap_node->node->rb_parent->rb_right == cur_swap_node->node)	// old_node is parent's right child
				cur_swap_node->node->rb_parent->rb_right = new_node;
			cur_swap_node->node->rb_parent = NULL;	// old_node's parent pointer is clear.

													//children pointer 
			if (cur_swap_node->node->rb_left != T->nil_node)	// old_node's left is exist
				cur_swap_node->node->rb_left->rb_parent = new_node;
			cur_swap_node->node->rb_left = NULL;	// old_node's left pointer is clear.
			if (cur_swap_node->node->rb_right != T->nil_node)	// old_node's right is exist
				cur_swap_node->node->rb_right->rb_parent = new_node;
			cur_swap_node->node->rb_right = NULL;	// old_node's left pointer is clear.
		}
		del_swap_node = cur_swap_node;
		cur_swap_node = cur_swap_node->next;
		del_swap_node->next->prev = del_swap_node->prev;
		del_swap_node->prev->next = del_swap_node->next;
		del_swap_node->node->rb_keys.next = NULL;
		del_swap_node->node->rb_keys.prev = NULL;

		free(del_swap_node->node);
		del_swap_node->next = NULL;
		del_swap_node->prev = NULL;
		del_swap_node->node = NULL;
		free(del_swap_node);
	}
}


	
void add_swap_list(simul_rb_node *node) {
	struct simul_swap_node *new_swap = NULL, *cur_node = swap_head.next;
	if (node->count < SWAP_COUNT)
		return;
	while (cur_node != &swap_head) {
		if (node->std_key == cur_node->node->std_key)
			return;
		cur_node = cur_node->next;
	}
	new_swap = (struct simul_swap_node *)malloc(sizeof(struct simul_swap_node));
	new_swap->node = node;
	new_swap->prev = &swap_head;
	new_swap->next = swap_head.next;
	new_swap->next->prev = new_swap;
	new_swap->prev->next = new_swap;
}

/*
simul_rb_node *simul_rb_swapping(simul_rb_root *T, simul_rb_node *old_node) {
	simul_rb_node *new_node = NULL;
	if (old_node->count < SWAP_COUNT)
		return old_node;
	swap_count++;
	// memory allocatioin and data copy 
	new_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));
	new_node->rb_color = old_node->rb_color;
	new_node->std_key = old_node->std_key;
	new_node->rb_keys.next = old_node->rb_keys.next;
	new_node->rb_keys.next->prev = &(new_node->rb_keys);
	new_node->rb_keys.prev = old_node->rb_keys.prev;
	new_node->rb_keys.prev->next = &(new_node->rb_keys);
	new_node->rb_parent = old_node->rb_parent;
	new_node->rb_left = old_node->rb_left;
	new_node->rb_right = old_node->rb_right;
	new_node->count = 1;

	// parent pointer 
	if (old_node->rb_parent == T->root_node)    // old_node is root
		T->root_node->rb_left = new_node;
	else if (old_node->rb_parent->rb_left == old_node)    // old_node is parent's left child
		old_node->rb_parent->rb_left = new_node;
	else if (old_node->rb_parent->rb_right == old_node)   // old_node is parent's right child
		old_node->rb_parent->rb_right = new_node;
	old_node->rb_parent = NULL;   // old_node's parent pointer is clear.

								  // children pointer 
	if (old_node->rb_left != T->nil_node)   // old_node's left is exist
		old_node->rb_left->rb_parent = new_node;
	old_node->rb_left = NULL;   // old_node's left pointer is clear.
	if (old_node->rb_right != T->nil_node)   // old_node's right is exist
		old_node->rb_right->rb_parent = new_node;
	old_node->rb_right = NULL;   // old_node's left pointer is clear.

	return new_node;
}
*/
int simul_rb_insert(simul_rb_root *T, long long int inkey) {
	int ret = 0, is_in_group = 0;
	simul_rb_node *comp_node = T->root_node->rb_left;
	simul_rb_node *in_parent_pos = T->root_node;
	simul_rb_node *new_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));
	long long int searching_inkey = inkey / GROUP_SIZE;
	simul_rb_key *cur_key = NULL, *pre_cur_key = NULL, *new_key = NULL;

	new_key = (simul_rb_key *)malloc(sizeof(simul_rb_key));
	init_rb_key(new_key, inkey);

	while (comp_node != T->nil_node) {
		in_parent_pos = comp_node;

		if (searching_inkey < comp_node->std_key)
			comp_node = left_of(comp_node);
		else if (searching_inkey > comp_node->std_key)
			comp_node = right_of(comp_node);
		else if (searching_inkey == comp_node->std_key) {
			pre_cur_key = &(comp_node->rb_keys);
			cur_key = container_of(comp_node->rb_keys.next, simul_rb_key, list);
			while (cur_key != &comp_node->rb_keys) {
				if (cur_key->rb_key == inkey)
					return SIMUL_RB_EXIST;
				else if (inkey > cur_key->rb_key) {
					pre_cur_key = cur_key;
					cur_key = container_of(cur_key->list.next, simul_rb_key, list);
				}
				else if (inkey < cur_key->rb_key) {
					is_in_group = 1;
					break;
				}
			}
			break;
		}
	}

	if (is_in_group == 1) {	// 그룹에만 추가
		list_add(&(new_key->list), &(pre_cur_key->list));
		return SIMUL_RB_IN_GROUP;
	}

	// 큰 노드 추가
	init_rb_node(T, new_node, new_key);

	new_node->rb_parent = in_parent_pos;

	if (in_parent_pos == T->root_node) {   /* the first insert (rbtree is empty) */
		T->root_node->rb_left = new_node;
		T->root_node->count++;   // for analysis
	}
	else if (new_node->std_key < in_parent_pos->std_key) {
		in_parent_pos->rb_left = new_node;
		in_parent_pos->count++;   // for analysis
	}
	else {
		in_parent_pos->rb_right = new_node;
		in_parent_pos->count++;   // for analysis
	}

	simul_rb_insert_fixup(T, new_node);

	return ret;
}

void simul_rb_insert_fixup(simul_rb_root *T, simul_rb_node *alt_node) {
	simul_rb_node *across_parent = NULL;

	while (is_rb_red(parent_of(alt_node))) {
		if (parent_of(alt_node) == parent_of(alt_node)->rb_parent->rb_left) {

			across_parent = parent_of(alt_node)->rb_parent->rb_right;

			if (is_rb_red(across_parent)) {   /* case 1: color of across is red */
				set_rb_black(parent_of(alt_node));
				set_rb_black(across_parent);
				set_rb_red(parent_of(alt_node->rb_parent));
				alt_node = parent_of(alt_node->rb_parent);
			}
			else {
				if (alt_node == parent_of(alt_node)->rb_right) {   /* case 2: color of across is black && alt_node is right child */
					alt_node = parent_of(alt_node);
					alt_node = simul_rb_left_rotate(T, alt_node);
				}
				/* case 3: color of across is black && alt_node is left child */
				set_rb_black(parent_of(alt_node));
				set_rb_red(parent_of(alt_node->rb_parent));
				simul_rb_right_rotate(T, parent_of(alt_node->rb_parent));
			}
		}
		else if (parent_of(alt_node) == parent_of(alt_node)->rb_parent->rb_right) {

			across_parent = parent_of(alt_node)->rb_parent->rb_left;

			if (is_rb_red(across_parent)) {   /* case 1: color of across is red */
				set_rb_black(parent_of(alt_node));
				set_rb_black(across_parent);
				set_rb_red(parent_of(alt_node->rb_parent));
				alt_node = parent_of(alt_node->rb_parent);
			}
			else {
				if (alt_node == parent_of(alt_node)->rb_left) {   /* case 2: color of across is black && alt_node is right child */
					alt_node = parent_of(alt_node);
					alt_node = simul_rb_right_rotate(T, alt_node);
				}
				/* case 3: color of across is black && alt_node is left child */
				set_rb_black(parent_of(alt_node));
				set_rb_red(parent_of(alt_node->rb_parent));
				simul_rb_left_rotate(T, parent_of(alt_node->rb_parent));
			}
		}
	}

	set_rb_black(T->root_node->rb_left); /* rule 2: color of root node is black */
}

int simul_rb_delete(simul_rb_root *T, long long int delkey) {
	int ret = 0, is_in_group = 0;
	simul_rb_node *del_node = simul_rb_search(T, delkey);
	simul_rb_node *y;
	simul_rb_node *x;
	simul_rb_key *pre_cur_key = NULL, *cur_key = NULL;

	if (del_node == T->nil_node)
		return SIMUL_RB_NOT_EXIST;

	pre_cur_key = &(del_node->rb_keys);
	cur_key = container_of(del_node->rb_keys.next, simul_rb_key, list);

	while (cur_key != &del_node->rb_keys) {
		if (cur_key->rb_key == delkey) {
			is_in_group = 1;
			break;
		}
		else if (delkey > cur_key->rb_key) {
			pre_cur_key = cur_key;
			cur_key = container_of(cur_key->list.next, simul_rb_key, list);
		}
		else if (delkey < cur_key->rb_key) {
			is_in_group = 0;
			return SIMUL_RB_NOT_EXIST;
		}
	}

	if (cur_key == &del_node->rb_keys)
		return SIMUL_RB_NOT_EXIST;

	if (is_in_group == 1) {
		list_del(&(cur_key->list));
	}

	if (&(del_node->rb_keys) != del_node->rb_keys.next)
		return SIMUL_RB_IN_GROUP;
	
	y = ((del_node->rb_left == T->nil_node) || (del_node->rb_right == T->nil_node)) ? del_node : simul_rb_successor(T, del_node);
	x = (y->rb_left == T->nil_node) ? y->rb_right : y->rb_left;

	x->count++;   // for analysis
	if (T->root_node == (x->rb_parent = y->rb_parent)) {/* assignment of y->p to x->p is intentional */
		T->root_node->rb_left = x;
		T->root_node->count++;   // for analysis
	}
	else {
		if (y == y->rb_parent->rb_left) {
			y->rb_parent->rb_left = x;
			y->rb_parent->count++;   // for analysis
		}
		else {
			y->rb_parent->rb_right = x;
			y->rb_parent->count++;   // for analysis
		}
	}

	if (y != del_node) {
		if (is_rb_black(y))
			simul_rb_delete_fixup(T, x);

		y->rb_left = del_node->rb_left;
		y->count++;   // for analysis
		y->rb_right = del_node->rb_right;
		y->count++;   // for analysis
		y->rb_parent = del_node->rb_parent;
		y->count++;   // for analysis
		y->rb_color = del_node->rb_color;
		del_node->rb_left->rb_parent = del_node->rb_right->rb_parent = y;
		del_node->rb_left->count++;      // for analysis
		del_node->rb_right->count++;   // for analysis
		if (del_node == del_node->rb_parent->rb_left) {
			del_node->rb_parent->rb_left = y;
			del_node->rb_parent->count++;   // for analysis
		}
		else {
			del_node->rb_parent->rb_right = y;
			del_node->rb_parent->count++;   // for analysis
		}
		//printf("addr %p\t count %u\n", del_node, del_node->count);
		//free(del_node);
		del_node->is_free = 1;
	}
	else {
		if (is_rb_black(y))
			simul_rb_delete_fixup(T, x);
		//printf("addr %p\t count %u\n", y, y->count);
		//free(y);
		y->is_free = 1;
	}

	return ret;
}

void simul_rb_delete_fixup(simul_rb_root *T, simul_rb_node *chg_node) {
	simul_rb_node *bro_node = NULL;
	while (chg_node != T->root_node && is_rb_black(chg_node)) {

		if (chg_node == parent_of(chg_node)->rb_left) {
			bro_node = parent_of(chg_node)->rb_right;

			if (is_rb_red(bro_node)) { /* case 1: color of bro node is red */
				set_rb_black(bro_node);
				set_rb_red(parent_of(chg_node));
				simul_rb_left_rotate(T, parent_of(chg_node));
				bro_node = parent_of(chg_node)->rb_right;
			}

			if (is_rb_black(left_of(bro_node)) && is_rb_black(right_of(bro_node))) {   /* case 2: color of bro node is black && both of bro's children are black */
				set_rb_red(bro_node);
				chg_node = parent_of(chg_node);

			}
			else {
				if (is_rb_black(right_of(bro_node))) {   /* case 3: bro is black && left child is red && right child is black */
					set_rb_black(left_of(bro_node));
					set_rb_red(bro_node);
					simul_rb_right_rotate(T, bro_node);
					bro_node = parent_of(chg_node)->rb_right;
				}
				bro_node->rb_color = parent_of(chg_node)->rb_color;
				set_rb_black(parent_of(chg_node));
				set_rb_black(right_of(bro_node));
				simul_rb_left_rotate(T, parent_of(chg_node));
				chg_node = T->root_node;
			}
		}
		else {
			bro_node = parent_of(chg_node)->rb_left;

			if (is_rb_red(bro_node)) { /* case 1: color of bro node is red */
				set_rb_black(bro_node);
				set_rb_red(parent_of(chg_node));
				simul_rb_right_rotate(T, parent_of(chg_node));
				bro_node = parent_of(chg_node)->rb_left;
			}

			if (is_rb_black(left_of(bro_node)) && is_rb_black(right_of(bro_node))) {   /* case 2: color of bro node is black && both of bro's children are black */
				set_rb_red(bro_node);
				chg_node = parent_of(chg_node);

			}
			else {
				if (is_rb_black(left_of(bro_node))) {   /* case 3: bro is black && right child is red && left child is black */
					set_rb_black(right_of(bro_node));
					set_rb_red(bro_node);
					simul_rb_left_rotate(T, bro_node);
					bro_node = parent_of(chg_node)->rb_left;
				}
				bro_node->rb_color = parent_of(chg_node)->rb_color;
				set_rb_black(parent_of(chg_node));
				set_rb_black(left_of(bro_node));
				simul_rb_right_rotate(T, parent_of(chg_node));
				chg_node = T->root_node;
			}
		}
	} //while
	T->nil_node->rb_parent = T->nil_node;
	T->nil_node->count++;   // for analysis
	T->nil_node->rb_left = T->nil_node;
	T->nil_node->count++;   // for analysis
	T->nil_node->rb_right = T->nil_node;
	T->nil_node->count++;   // for analysis

	set_rb_black(chg_node);
}

simul_rb_node *simul_rb_search(simul_rb_root *T, long long int find_key) {
	simul_rb_node *search_node = T->root_node->rb_left;
	long long int find_std_key = find_key / GROUP_SIZE;

	while (search_node != T->nil_node) {
		if (find_std_key == search_node->std_key)
			break;

		if (find_std_key < search_node->std_key)
			search_node = left_of(search_node);
		else
			search_node = right_of(search_node);
	}

	return search_node;
}
void simul_rb_pre_order(simul_rb_root *T, simul_rb_node *node) {
	//printf("addr %p \tcount %u\n", node, node->count);
	if (node->rb_left != T->nil_node)
		simul_rb_pre_order(T, node->rb_left);
	if (node->rb_right != T->nil_node)
		simul_rb_pre_order(T, node->rb_right);
}

void simul_rb_delete_all(simul_rb_root *T, simul_rb_node *node) {
	if (node->rb_left != T->nil_node)
		simul_rb_delete_all(T, node->rb_left);
	if (node->rb_right != T->nil_node)
		simul_rb_delete_all(T, node->rb_right);
	free(node);
}


int main(int argc, char *argv[])
{
	simul_rb_root *root = simul_rb_alloc_root();
	simul_rb_node *node = NULL;
	FILE *fp = NULL;
	long long int in_key = -1, i = 0;
	char in_type[20], fn[40];
	int retval = -1, endpoint = 0, line = 1000;
	int *a = NULL, *b = NULL, *c = NULL, *d = NULL, *e = NULL, *f = NULL;

	/*
	init_free_area(free_area);
	init_alloc_area(alloc_area);
	a = smalloc(5);
	b = smalloc(5);
	c = smalloc(5);
	f = smalloc(5);
	d = smalloc(5);
	e = smalloc(5);
	sfree(a);
	sfree(b);
	sfree(c);
	sfree(d);
	sfree(e);
	sfree(f);
	*/

	/*
	if (argc < 2) {
	printf (" execute error.\n");
	printf (" usage : %s key_file\n", argv[0]);
	return 1;
	}
	*/

	//strcpy (fn, argv[1]);
	strcpy(fn, "trace2.result");

	while (1) {
		swap_count = 0;
		init_swap_node();
		init_rb_root(root);
		fp = fopen(fn, "r");
		printf("line %d\n", line);
		for (i = 0; i < line; i++) {
			fscanf(fp, "%s %llu\n", in_type, &in_key);
			if (strcmp(in_type, "insert") == 0)
				retval = simul_rb_insert(root, in_key);
			if (strcmp(in_type, "erase") == 0)
				retval = simul_rb_delete(root, in_key);
			if (feof(fp))
				endpoint = 1;
			simul_rb_swapping(root);
		}
		line += 1000;
		fclose(fp);
		//printf("start pre order\n");
		simul_rb_pre_order(root, root->root_node->rb_left);
		//printf("end pre order\n");
		simul_rb_delete_all(root, root->root_node->rb_left);
		printf("end %d\n", line - 1000);
		printf("swap_count : %llu\n", swap_count);
		if (endpoint == 1)
			break;
		
	}

	return 0;
}