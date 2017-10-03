#include "evenly_buddy_allocator.h"
#include<stdio.h>

//extern struct simul_free_area free_area[SIMUL_MAX_ORDER];
//extern struct simul_free_area alloc_area[SIMUL_MAX_ORDER];

void for_debug_init_sfull_memory();
void for_debug_free_area();
void for_debug_sfull_memory();


int main(void) {
	int *a = NULL, *b = NULL, *c = NULL, *d = NULL, *e = NULL, *f = NULL, *g = NULL, *h = NULL;

	for_debug_init_sfull_memory();

	init_free_area(free_area);
	init_alloc_area(alloc_area);

	//for_debug_free_area();

	
	a = smalloc(5);
	b = smalloc(5);	
	c = smalloc(5);	
	d = smalloc(5);
	e = smalloc(5);
	f = smalloc(5);
	g = smalloc(5);
	h = smalloc(5);

	sfree(e);
	sfree(f);
	for_debug_free_area();
	sfree(a);
	for_debug_free_area();
	sfree(b);
	for_debug_free_area();
	sfree(c);
	sfree(d);
	sfree(g);
	sfree(h);
	f = smalloc(5);
	a = smalloc(5);
	d = smalloc(5);
	c = smalloc(5);
	e = smalloc(5);
	b = smalloc(5);
	sfree(a);
	sfree(b);
	sfree(c);
	sfree(d);
	sfree(e);
	sfree(f);
	

	

	return 0;
}

void for_debug_init_sfull_memory() {
	int i = 0;
	for (i = 0; i < MEM_POOL_SIZE; i++)
		sfull_mem[i] = i;
}
void for_debug_free_area() {
	int i = 0;
	struct simul_area *area = NULL;
	struct simul_list_head *cur_list = NULL;
	for (i = 0; i < SIMUL_MAX_ORDER; i++) {
		cur_list = free_area[i].list.next;
		printf("level : %d ", i);
		while (cur_list != &(free_area[i].list)) {
			area = container_of(cur_list, struct simul_area, list);
			printf("start : %u, end : %u, addr : %u, write : %u ", area->start, area->end, *(area->addr), area->write);
			cur_list = cur_list->next;
		}
		printf("\n");
	}
}
void for_debug_sfull_memory() {
	int i = 0;
	for (i = 0; i < MEM_POOL_SIZE; i++) {
		printf("start : %u, addr : %u ", i, sfull_mem[i]);
	}
}