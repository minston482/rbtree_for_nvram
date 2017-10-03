#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree_per_pointer_extract.h"

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

void init_rb_node(simul_rb_root *T, simul_rb_node *node, long long int key) {
	set_rb_red(node);
	node->rb_key = key;
	node->rb_parent = T->nil_node;
	node->rb_right = T->nil_node;
	node->rb_left = T->nil_node;
	node->count.parent = 1;	// for analysis
	node->count.left = 1;	// for analysis
	node->count.right = 1;	// for analysis
}

void init_rb_root(simul_rb_root *root) {
	root->nil_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));
	root->root_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));

	set_rb_black(root->nil_node);
	root->nil_node->rb_key = SIMUL_RB_NIL;
	root->nil_node->rb_parent = root->nil_node;
	root->nil_node->rb_right = root->nil_node;
	root->nil_node->rb_left = root->nil_node;
	root->nil_node->count.parent = 1;	// for analysis
	root->nil_node->count.left = 1;	// for analysis
	root->nil_node->count.right = 1;	// for analysis
	set_rb_black(root->root_node);
	root->root_node->rb_key = SIMUL_RB_NIL;
	root->root_node->rb_parent = root->nil_node;
	root->root_node->rb_left = root->nil_node;
	root->root_node->rb_right = root->nil_node;
	root->root_node->count.parent = 1;	// for analysis
	root->root_node->count.left = 1;	// for analysis
	root->root_node->count.right = 1;	// for analysis
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

unsigned int simul_rb_left_rotate(simul_rb_root *T, simul_rb_node *node) {
	unsigned int ret = 0;
	simul_rb_node *right_node = right_of(node);

	/* exchange left child of node_right to right child of node */
	node->rb_right = left_of(right_node);
	node->count.right++;	// for analysis
	if (left_of(right_node) != T->nil_node) {
		left_of(right_node)->rb_parent = node;
		left_of(right_node)->count.parent++;	// for analysis
	}

	/* change parent connection */
	right_node->rb_parent = parent_of(node);
	right_node->count.parent++;		// for analysis

	if (node == parent_of(node)->rb_left) {	/* case: node is left child of parent */
		parent_of(node)->rb_left = right_node;
		parent_of(node)->count.left++;	// for analysis
	}
	else if (node == parent_of(node)->rb_right) {	/* case: node is right child of parent */
		parent_of(node)->rb_right = right_node;
		parent_of(node)->count.right++;	// for analysis
	}

	right_node->rb_left = node;
	right_node->count.left++;	// for analysis
	node->rb_parent = right_node;
	node->count.parent++;	// for analysis

	return ret;
}

unsigned int simul_rb_right_rotate(simul_rb_root *T, simul_rb_node *node) {
	unsigned int ret = 0;
	simul_rb_node *left_node = left_of(node);

	/* exchange right child of node_left to left child of node */
	node->rb_left = right_of(left_node);
	node->count.left++;	// for analysis
	if (right_of(left_node) != T->nil_node) {
		right_of(left_node)->rb_parent = node;
		right_of(left_node)->count.parent++;	// for analysis
	}

	/* change parent connection */
	left_node->rb_parent = parent_of(node);
	left_node->count.parent++;	// for analysis

	if (node == parent_of(node)->rb_left) {	/* case: node is left child of parent */
		parent_of(node)->rb_left = left_node;
		parent_of(node)->count.left++;	// for analysis
	}
	else if (node == parent_of(node)->rb_right) {	/* case: node is right child of parent */
		parent_of(node)->rb_right = left_node;
		parent_of(node)->count.right++;	// for analysis
	}

	left_node->rb_right = node;
	left_node->count.right++;	// for analysis
	node->rb_parent = left_node;
	node->count.parent++;	// for analysis

	return ret;
}

/* change parent of chg_node from parent of pre_node */
unsigned int simul_rb_transplant(simul_rb_root *T, simul_rb_node *pre_node, simul_rb_node *chg_node) {
	unsigned int ret = 0;

	if (parent_of(pre_node) == T->nil_node)	/* case: node is root node */
		T->root_node = chg_node;
	else if (pre_node == parent_of(pre_node)->rb_left)	/* case: node is left child of parent */
		parent_of(pre_node)->rb_left = chg_node;
	else if (pre_node == parent_of(pre_node)->rb_right)	/* case: node is right child of parent */
		parent_of(pre_node)->rb_right = chg_node;

	chg_node->rb_parent = parent_of(pre_node);	/* change chg_node's parent */

	return ret;
}

simul_rb_node *simul_rb_minimum(simul_rb_node *node) {
	simul_rb_node *min_node = node;
	while (left_of(min_node)->rb_key != SIMUL_RB_NIL)
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

int simul_rb_insert(simul_rb_root *T, long long int inkey) {
	int ret = 0;
	simul_rb_node *comp_node = T->root_node->rb_left;
	simul_rb_node *in_parent_pos = T->root_node;
	simul_rb_node *new_node = (simul_rb_node *)malloc(sizeof(simul_rb_node));

	while (comp_node != T->nil_node) {
		in_parent_pos = comp_node;

		if (inkey < comp_node->rb_key)
			comp_node = left_of(comp_node);
		else if (inkey > comp_node->rb_key)
			comp_node = right_of(comp_node);
		else if (inkey == comp_node->rb_key)
			return SIMUL_RB_EXIST;
	}
	
	init_rb_node(T, new_node, inkey);

	new_node->rb_parent = in_parent_pos;

	if (in_parent_pos == T->root_node) {	/* the first insert (rbtree is empty) */
		T->root_node->rb_left = new_node;
		T->root_node->count.left++;	// for analysis
	}
	else if (new_node->rb_key < in_parent_pos->rb_key) {
		in_parent_pos->rb_left = new_node;
		in_parent_pos->count.left++;	// for analysis
	}
	else {
		in_parent_pos->rb_right = new_node;
		in_parent_pos->count.right++;	// for analysis
	}

	simul_rb_insert_fixup(T, new_node);

	return ret;
}

void simul_rb_insert_fixup(simul_rb_root *T, simul_rb_node *alt_node) {
	simul_rb_node *across_parent = NULL;

	while (is_rb_red(parent_of(alt_node))) {
		if (parent_of(alt_node) == parent_of(alt_node)->rb_parent->rb_left) {

			across_parent = parent_of(alt_node)->rb_parent->rb_right;

			if (is_rb_red(across_parent)) {	/* case 1: color of across is red */
				set_rb_black(parent_of(alt_node));
				set_rb_black(across_parent);
				set_rb_red(parent_of(alt_node->rb_parent));
				alt_node = parent_of(alt_node->rb_parent);
			}
			else {
				if (alt_node == parent_of(alt_node)->rb_right) {	/* case 2: color of across is black && alt_node is right child */
					alt_node = parent_of(alt_node);
					simul_rb_left_rotate(T, alt_node);
				}
				/* case 3: color of across is black && alt_node is left child */
				set_rb_black(parent_of(alt_node));
				set_rb_red(parent_of(alt_node->rb_parent));
				simul_rb_right_rotate(T, parent_of(alt_node->rb_parent));
			}
		}
		else if (parent_of(alt_node) == parent_of(alt_node)->rb_parent->rb_right) {

			across_parent = parent_of(alt_node)->rb_parent->rb_left;

			if (is_rb_red(across_parent)) {	/* case 1: color of across is red */
				set_rb_black(parent_of(alt_node));
				set_rb_black(across_parent);
				set_rb_red(parent_of(alt_node->rb_parent));
				alt_node = parent_of(alt_node->rb_parent);
			}
			else {
				if (alt_node == parent_of(alt_node)->rb_left) {	/* case 2: color of across is black && alt_node is right child */
					alt_node = parent_of(alt_node);
					simul_rb_right_rotate(T, alt_node);
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
	int ret = 0;
	simul_rb_node *del_node = simul_rb_search(T, delkey);
	simul_rb_node *y;
	simul_rb_node *x;

	if (del_node == T->nil_node)
		return SIMUL_RB_NOT_EXIST;

	y = ((del_node->rb_left == T->nil_node) || (del_node->rb_right == T->nil_node)) ? del_node : simul_rb_successor(T, del_node);
	x = (y->rb_left == T->nil_node) ? y->rb_right : y->rb_left;

	x->count.parent++;	// for analysis
	if (T->root_node == (x->rb_parent = y->rb_parent)) {/* assignment of y->p to x->p is intentional */
		T->root_node->rb_left = x;
		T->root_node->count.left++;	// for analysis
	}
	else {
		if (y == y->rb_parent->rb_left) {
			y->rb_parent->rb_left = x;
			y->rb_parent->count.left++;	// for analysis
		}
		else {
			y->rb_parent->rb_right = x;
			y->rb_parent->count.right++;	// for analysis
		}
	}

	if (y != del_node) {
		if (is_rb_black(y))
			simul_rb_delete_fixup(T, x);

		y->rb_left = del_node->rb_left;
		y->count.left++;	// for analysis
		y->rb_right = del_node->rb_right;
		y->count.right++;	// for analysis
		y->rb_parent = del_node->rb_parent;
		y->count.parent++;	// for analysis
		y->rb_color = del_node->rb_color;
		del_node->rb_left->rb_parent = del_node->rb_right->rb_parent = y;
		del_node->rb_left->count.parent++;	// for analysis
		del_node->rb_right->count.parent++;	// for analysis
		if (del_node == del_node->rb_parent->rb_left) {
			del_node->rb_parent->rb_left = y;
			del_node->rb_parent->count.left++;	// for analysis
		}
		else {
			del_node->rb_parent->rb_right = y;
			del_node->rb_parent->count.right++;	// for analysis
		}
		printf("key %lld color %s\tparent %u \tleft %u \tright %u\n", del_node->rb_key, del_node->rb_color == SIMUL_RB_RED ? "red" : "black", del_node->count.parent, del_node->count.left, del_node->count.right);
		free(del_node);
	}
	else {
		if (is_rb_black(y))
			simul_rb_delete_fixup(T, x);
		printf("key %lld color %s\tparent %u \tleft %u \tright %u\n", y->rb_key, y->rb_color == SIMUL_RB_RED ? "red" : "black", y->count.parent, y->count.left, y->count.right);
		free(y);
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

			if (is_rb_black(left_of(bro_node)) && is_rb_black(right_of(bro_node))) {	/* case 2: color of bro node is black && both of bro's children are black */
				set_rb_red(bro_node);
				chg_node = parent_of(chg_node);

			}
			else {
				if (is_rb_black(right_of(bro_node))) {	/* case 3: bro is black && left child is red && right child is black */
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

			if (is_rb_black(left_of(bro_node)) && is_rb_black(right_of(bro_node))) {	/* case 2: color of bro node is black && both of bro's children are black */
				set_rb_red(bro_node);
				chg_node = parent_of(chg_node);

			}
			else {
				if (is_rb_black(left_of(bro_node))) {	/* case 3: bro is black && right child is red && left child is black */
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
	T->nil_node->count.parent++;	// for analysis
	T->nil_node->rb_left = T->nil_node;
	T->nil_node->count.left++;	// for analysis
	T->nil_node->rb_right = T->nil_node;
	T->nil_node->count.right++;	// for analysis

	set_rb_black(chg_node);
}

simul_rb_node *simul_rb_search(simul_rb_root *T, long long int find_key) {
	simul_rb_node *search_node = T->root_node->rb_left;

	while (search_node != T->nil_node) {
		if (find_key == search_node->rb_key)
			break;

		if (find_key < search_node->rb_key)
			search_node = left_of(search_node);
		else
			search_node = right_of(search_node);
	}

	return search_node;
}
void simul_rb_pre_order(simul_rb_root *T, simul_rb_node *node) {
	printf("key %lld color %s\tparent %u \tleft %u \tright %u\n", node->rb_key, node->rb_color == SIMUL_RB_RED ? "red" : "black", node->count.parent, node->count.left, node->count.right);
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
	
	if (argc < 2) {
		printf (" execute error.\n");
		printf (" usage : %s key_file\n", argv[0]);
		return 1;
	}
	

	strcpy (fn, argv[1]);

	while(1) {
		init_rb_root(root);
		fp = fopen (fn, "r");
		printf("line %d\n", line);
		for (i = 0; i < line; i++) {
	
			fscanf (fp, "%s %llu\n", in_type, &in_key);
			if (strcmp (in_type, "insert") == 0)
				retval = simul_rb_insert (root, in_key);
			if (strcmp (in_type, "erase") == 0)
				retval = simul_rb_delete (root, in_key);
			if (feof(fp))
				endpoint = 1;
		}
		line += 1000;
		fclose(fp);
		simul_rb_pre_order(root, root->root_node->rb_left);
		simul_rb_delete_all(root, root->root_node->rb_left);
		printf("end %d\n", line-1000);
		if (endpoint == 1)
			break;
	}

	return 0;
}
