#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char fn[50], open_exact_fn[100], open_swap_fn[100], comb_fn[100];
	int term = atoi(argv[2]), count = 0, fcount = 0;
	FILE *exact_fp = NULL, *swap_fp = NULL, *comb_fp = NULL;
	char addr[100];

	strcpy (fn, argv[1]);

	while (1) {
		fcount += term;
		sprintf(open_exact_fn, "%s.exact.%d.rst.sum", fn, fcount);
		sprintf(open_swap_fn, "%s.swap.%d.rst.sum", fn, fcount);
		sprintf(comb_fn, "%s.comb.%d.rst", fn, fcount);
		
		exact_fp = fopen (open_exact_fn, "r");
		swap_fp = fopen (open_swap_fn, "r");
		if (exact_fp == NULL || swap_fp == NULL) {
			break;
		}
		comb_fp = fopen (comb_fn, "w");
		while (!feof(exact_fp) || !feof(swap_fp)) {
			fscanf(exact_fp, "%s %d\n", addr, &count);
			fprintf(comb_fp, "%d ", count);
			fscanf(swap_fp, "%s %d\n", addr, &count);
			fprintf(comb_fp, "%d\n", count);
		}
		
		fclose (comb_fp);
		fclose (exact_fp);
		fclose (swap_fp);
	}

	return 0;
}
