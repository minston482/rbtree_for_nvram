#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evenly_buddy_allocator.h"

extern sfull_mem[MEM_POOL_SIZE];
extern struct simul_free_area free_area[SIMUL_MAX_ORDER];
extern struct simul_free_area alloc_area[SIMUL_MAX_ORDER];

void init_free_area(struct simul_free_area *free_area) {
	int i = 0;
	struct simul_area *area = NULL;

	for (i = 0; i < SIMUL_MAX_ORDER; i++) {
		init_list_head(&(free_area[i].list));
		free_area[i].nr_free = 0;
	}
	area = (struct simul_area *)malloc(sizeof(struct simul_area));
	area->start = 0;
	area->end = MEM_POOL_SIZE-1;
	area->write = sum_write(area->start, area->end);
	area->addr = sfull_mem;
	init_list_head(&(area->list));
	list_add(&(area->list), &(free_area[i-1].list));
	free_area[i - 1].nr_free = 1;
}

void init_alloc_area(struct simul_free_area *alloc_area) {
	int i = 0;
	for (i = 0; i < SIMUL_MAX_ORDER; i++) {
		init_list_head(&(alloc_area[i].list));
		alloc_area[i].nr_free = 0;
	}
}

void *smalloc(unsigned int size) {
	void *retval = NULL;
	int i = 0, start_index = size / NODE_SIZE;
	struct simul_area *del_area = NULL, *area = NULL;
	struct simul_list_head *to_insert_list = NULL;

	for (i = start_index; i < SIMUL_MAX_ORDER; i++) {
		if (free_area[i].nr_free == 0)
			continue;
		else
			break;
	}
	if (i == SIMUL_MAX_ORDER)
		i--;

	for (; i > start_index; i--) {		// 상위 레벨에서부터 요구한 레벨까지 내려가며 노드 분할
		del_area = container_of(free_area[i].list.next, struct simul_area, list);

		area = (struct simul_area *)malloc(sizeof(struct simul_area));
		area->start = ((del_area->start + del_area->end) / 2) + 1;
		area->end = del_area->end;

		area->addr = &(sfull_mem[area->start]);
		area->write = sum_write(area->start, area->end);
		to_insert_list = find_insert_pos(area, i-1);

		list_add(&(area->list), to_insert_list);
		free_area[i - 1].nr_free++;

		area = (struct simul_area *)malloc(sizeof(struct simul_area));
		area->start = del_area->start;
		area->end = (del_area->start + del_area->end) / 2;

		area->addr = &(sfull_mem[area->start]);
		area->write = sum_write(area->start, area->end);
		to_insert_list = find_insert_pos(area, i-1);

		list_add(&(area->list), to_insert_list);
		free_area[i - 1].nr_free++;

		list_del(free_area[i].list.next);
		free_area[i].nr_free--;
		free(del_area);
	}

	del_area = container_of(free_area[i].list.next, struct simul_area, list);
	list_del(free_area[i].list.next);
	free_area[i].nr_free--;
	list_add(&(del_area->list), &(alloc_area[i].list));
	alloc_area[i].nr_free++;

	//retval = &sfull_mem[del_area->start];
	retval = (void *)del_area;

	return retval;
}

void sfree(void *block) {
	struct simul_area *del_area = (struct simul_area *)block, *area = NULL, *add_area = NULL;
	int i = 0, start_index = sizeof(del_area) / NODE_SIZE;
	int del_start = -1, is_even_start = 0, is_merge_end = 0;
	struct simul_list_head *to_insert_list = NULL;
	
	list_del(&(del_area->list));
	alloc_area[start_index].nr_free--;

	to_insert_list = find_insert_pos(del_area, start_index);
	list_add(&(del_area->list), to_insert_list);
	//list_add(&(del_area->list), &(free_area[start_index].list));	// free list에 1개 추가
	free_area[start_index].nr_free++;

	if(free_area[start_index].nr_free != 1) {	// 기존에 이미 있는 경우 합칠꺼 합친다.
		while (is_merge_end == 0) {	// level 을 올라가며 합칠꺼 합치는 loop
			area = container_of(free_area[start_index].list.next, struct simul_area, list);
			
			is_even_start = 0;
			del_start = del_area->start / (int)pow((int)2, (int)start_index);
			if (del_start % 2 == 0)
				is_even_start = 1;

			if (is_even_start == 1) {	// 합쳐져야 할게 짝수인 경우 ex) 0, 2, 4, 6 등.. 으로 시작하여 다음 인접한 홀수를 찾을때
				do {	// 해당 레벨에서 합칠꺼 있는지 확인
					if (del_start + 1 == (area->start)/((int)pow((int)2, (int)start_index))) {	// 합칠께 존재 할 경우
						list_del(&(del_area->list));
						free_area[start_index].nr_free--;
						list_del(&(area->list));
						free_area[start_index].nr_free--;
						add_area = (struct simul_area *)malloc(sizeof(struct simul_area));
						add_area->start = del_area->start;
						add_area->end = area->end;

						add_area->addr = &(sfull_mem[add_area->start]);
						add_area->write = sum_write(add_area->start, add_area->end);
						to_insert_list = find_insert_pos(add_area, start_index+1);

						list_add(&(add_area->list), to_insert_list);
						free_area[start_index + 1].nr_free++;
						is_merge_end = 0;
						break;
					}
					else {
						area = area->list.next;
					}
				} while (area != &(free_area[start_index].list));

				if (area != &(free_area[start_index].list)){	// 합쳐져서 계속 레벨을 올라가야 하는 경우
					start_index++;
					is_merge_end = 0;
					del_area = add_area;
					continue;
				}
				else {	// 더이상 merge가 필요없음
					is_merge_end = 1;
				}
			}
			else if (is_even_start == 0) {	// 합쳐져야 할게 홀수인 경우 ex) 1, 3, 5, 7 등.. 으로 시작하여 이전 인접한 짝수를 찾을때
				do {	// 해당 레벨에서 합칠꺼 있는지 확인
					if (del_start - 1 == area->start / ((int)pow((int)2, (int)start_index))) {	// 합칠께 존재 할 경우
						list_del(&(del_area->list));
						free_area[start_index].nr_free--;
						list_del(&(area->list));
						free_area[start_index].nr_free--;
						add_area = (struct simul_area *)malloc(sizeof(struct simul_area));
						add_area->start = area->start;
						add_area->end = del_area->end;

						add_area->addr = &(sfull_mem[add_area->start]);
						add_area->write = sum_write(add_area->start, add_area->end);
						to_insert_list = find_insert_pos(add_area, start_index + 1);

						list_add(&(add_area->list), to_insert_list);
						free_area[start_index + 1].nr_free++;
						is_merge_end = 0;
						break;
					}
					else {
						area = area->list.next;
					}
				} while (area != &(free_area[start_index].list));

				if (area != &(free_area[start_index].list)) {	// 합쳐져서 계속 레벨을 올라가야 하는 경우
					start_index++;
					is_merge_end = 0;
					del_area = add_area;
					continue;
				}
				else {	// 더이상 merge가 필요없음
					is_merge_end = 1;
				}
			} // else if (is_even_start)
		} // while (is_merge_end == 1)
	} // if (free_area[start_index].nr_free != 1)

	//free(block);
}

unsigned long sum_write(unsigned long left, unsigned long right) {
	unsigned long retval = 0;
	int i = 0;

	for (i = left; i <= right; i++)
		retval += sfull_mem[i];

	return retval;
}

struct simul_list_head* find_insert_pos(struct simul_area *insert_area, int level) {
	struct simul_area *first_area = container_of(free_area[level].list.next, struct simul_area, list);
	struct simul_list_head *max_list = free_area[level].list.next, *retval = &(free_area[level].list);
	struct simul_area *max_area = container_of(max_list, struct simul_area, list);
	int is_first = 1;

	if (max_list == &(free_area[level].list))
		return retval;

	do {
		if (insert_area->write <= max_area->write) {
			break;
		}
		else {
			retval = max_list;
			max_list = max_list->next;
			max_area = container_of(max_list->next, struct simul_area, list);
			is_first = 0;
		}
	} while (max_list != &(free_area[level].list));

	return retval;
}