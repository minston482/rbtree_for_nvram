#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	long long int key, count;
	char tmp1[5], tmp2[10], in_fn[50], out_fn[50], num[1000];
	FILE *in_fp = NULL, *out_fp = NULL;
	char addr[100];
	
	strcpy (in_fn, argv[1]);
	in_fp = fopen (in_fn, "r");

	while (!feof (in_fp)) {
		fscanf (in_fp, "%s ", tmp1);
		if (strcmp (tmp1, "line") == 0) {
			fscanf (in_fp, "%llu", &key);
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
			fscanf (in_fp, "%llu\n", &key);
		}
		else {
			fscanf (in_fp, "%s %s %llu\n", addr, tmp2, &count);
			fprintf (out_fp, "%s %llu\n", addr, count);
		}
	}

	fclose (in_fp);

	return 0;
}
