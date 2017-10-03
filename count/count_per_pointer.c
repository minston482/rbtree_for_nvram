#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	long long int key, parent, left, right;
	char tmp1[5], tmp2[10], tmp3[10], tmp4[10], tmp5[10], tmp6[10], in_fn[50], out_fn[50], num[1000];
	FILE *in_fp = NULL, *out_fp = NULL;
	int i = 0, line = 0;
	
	strcpy (in_fn, argv[1]);
	in_fp = fopen (in_fn, "r");

	while (!feof (in_fp)) {
		fscanf (in_fp, "%s %llu", tmp1, &key);
		if (strcmp (tmp1, "line") == 0) {
			strcpy (out_fn, in_fn);
			strcat (out_fn, ".");
			sprintf (num, "%llu", key);
			strcat (out_fn, num);
			strcat (out_fn, ".rst");
			out_fp = fopen (out_fn, "w");
			fscanf (in_fp, "\n");
		}
		else if (strcmp (tmp1, "end") == 0) {
			fclose (out_fp);
			fscanf (in_fp, "\n");
		}
		else {
			fscanf (in_fp, "%s %s %s %llu %s %llu %s %llu\n", tmp2, tmp3, tmp4, &parent, tmp5, &left, tmp6, &right);
			fprintf (out_fp, "%llu %llu %llu\n", parent, left, right);
		}
	}

	fclose (in_fp);

	return 0;
}
