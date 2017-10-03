#pragma once
#include "linked_list.h"
#define SIMUL_MAX_ORDER   11
//#define MEM_POOL_SIZE   (4*1024*1024*1024)
#define MEM_POOL_SIZE   (1024)
#define NODE_SIZE   4096

unsigned long sfull_mem[MEM_POOL_SIZE];

struct simul_free_area {
	struct simul_list_head list;
	unsigned long nr_free;
};

struct simul_area {
	struct simul_list_head list;
	unsigned long start;
	unsigned long end;
	unsigned long write;
	unsigned long *addr;
};

struct simul_free_area free_area[SIMUL_MAX_ORDER];
struct simul_free_area alloc_area[SIMUL_MAX_ORDER];

void init_free_area(struct simul_free_area *free_area);
void *smalloc(unsigned int size);
void sfree(void *block);
unsigned long sum_write(unsigned long left, unsigned long right);
struct simul_list_head* find_insert_pos(struct simul_area *insert_area, int level);
