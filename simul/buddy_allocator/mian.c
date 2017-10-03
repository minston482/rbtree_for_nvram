#include "buddy_allocator.h"
#include<stdio.h>

//extern struct simul_free_area free_area[SIMUL_MAX_ORDER];
//extern struct simul_free_area alloc_area[SIMUL_MAX_ORDER];

int main(void) {
	int *a = NULL, *b = NULL, *c = NULL, *d = NULL, *e = NULL, *f = NULL;
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
	a = smalloc(5);
	c = smalloc(5);
	f = smalloc(5);
	e = smalloc(5);
	b = smalloc(5);
	d = smalloc(5);
	sfree(a);
	sfree(b);
	sfree(c);
	sfree(d);
	sfree(e);
	sfree(f);
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