#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "buddy_allocator.h"

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

	for (i = start_index; i < SIMUL_MAX_ORDER; i++) {
		if (free_area[i].nr_free == 0)
			continue;
		else
			break;
	}
	if (i == SIMUL_MAX_ORDER)
		i--;

	for (; i > start_index; i--) {		// ���� ������������ �䱸�� �������� �������� ��� ����
		del_area = container_of(free_area[i].list.next, struct simul_area, list);

		area = (struct simul_area *)malloc(sizeof(struct simul_area));
		area->start = ((del_area->start + del_area->end) / 2) + 1;
		area->end = del_area->end;
		list_add(&(area->list), &(free_area[i - 1].list));
		free_area[i - 1].nr_free++;

		area = (struct simul_area *)malloc(sizeof(struct simul_area));
		area->start = del_area->start;
		area->end = (del_area->start + del_area->end) / 2;
		list_add(&(area->list), &(free_area[i - 1].list));
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
	
	list_del(&(del_area->list));
	alloc_area[start_index].nr_free--;

	list_add(&(del_area->list), &(free_area[start_index].list));	// free list�� 1�� �߰�
	free_area[start_index].nr_free++;

	if(free_area[start_index].nr_free != 1) {	// ������ �̹� �ִ� ��� ��ĥ�� ��ģ��.
		while (is_merge_end == 0) {	// level �� �ö󰡸� ��ĥ�� ��ġ�� loop
			area = container_of(free_area[start_index].list.next, struct simul_area, list);
			
			is_even_start = 0;
			del_start = del_area->start / (int)pow((int)2, (int)start_index);
			if (del_start % 2 == 0)
				is_even_start = 1;

			if (is_even_start == 1) {	// �������� �Ұ� ¦���� ��� ex) 0, 2, 4, 6 ��.. ���� �����Ͽ� ���� ������ Ȧ���� ã����
				do {	// �ش� �������� ��ĥ�� �ִ��� Ȯ��
					if (del_start + 1 == (area->start)/((int)pow((int)2, (int)start_index))) {	// ��ĥ�� ���� �� ���
						list_del(&(del_area->list));
						free_area[start_index].nr_free--;
						list_del(&(area->list));
						free_area[start_index].nr_free--;
						add_area = (struct simul_area *)malloc(sizeof(struct simul_area));
						add_area->start = del_area->start;
						add_area->end = area->end;
						list_add(&(add_area->list), &(free_area[start_index + 1].list));
						free_area[start_index + 1].nr_free++;
						is_merge_end = 0;
						break;
					}
					else {
						area = area->list.next;
					}
				} while (area != &(free_area[start_index].list));

				if (area != &(free_area[start_index].list)){	// �������� ��� ������ �ö󰡾� �ϴ� ���
					start_index++;
					is_merge_end = 0;
					del_area = add_area;
					continue;
				}
				else {	// ���̻� merge�� �ʿ����
					is_merge_end = 1;
				}
			}
			else if (is_even_start == 0) {	// �������� �Ұ� Ȧ���� ��� ex) 1, 3, 5, 7 ��.. ���� �����Ͽ� ���� ������ ¦���� ã����
				do {	// �ش� �������� ��ĥ�� �ִ��� Ȯ��
					if (del_start - 1 == area->start / ((int)pow((int)2, (int)start_index))) {	// ��ĥ�� ���� �� ���
						list_del(&(del_area->list));
						free_area[start_index].nr_free--;
						list_del(&(area->list));
						free_area[start_index].nr_free--;
						add_area = (struct simul_area *)malloc(sizeof(struct simul_area));
						add_area->start = area->start;
						add_area->end = del_area->end;
						list_add(&(add_area->list), &(free_area[start_index + 1].list));
						free_area[start_index + 1].nr_free++;
						is_merge_end = 0;
						break;
					}
					else {
						area = area->list.next;
					}
				} while (area != &(free_area[start_index].list));

				if (area != &(free_area[start_index].list)) {	// �������� ��� ������ �ö󰡾� �ϴ� ���
					start_index++;
					is_merge_end = 0;
					del_area = add_area;
					continue;
				}
				else {	// ���̻� merge�� �ʿ����
					is_merge_end = 1;
				}
			} // else if (is_even_start)
		} // while (is_merge_end == 1)
	} // if (free_area[start_index].nr_free != 1)

	//free(block);
}