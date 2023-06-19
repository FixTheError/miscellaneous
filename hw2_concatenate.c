//Ryan Whitt
//CS-314
//Homework 2
#include <stdio.h>
#include <stdlib.h>

void concatenate(FILE** fp, int* lc, char** argv_1) {
	char* Bertha = NULL;
	size_t length = 0;
	int numread;
	while ((numread = getline(&Bertha, &length, *fp)) != -1) {
		if (atoi(*argv_1) == atoi("-b")) {
			fprintf(stdout, "%d ", *lc);
		}
		fprintf(stdout, "%s", Bertha);
		*lc++;

	}
}

int main(int argc, char* argv[]) {
	FILE* fp;
	int lc = 1;
	int i = 1;
	if (atoi(argv[i]) == atoi("-b")) { //getopt
		++i;
		while (argv[i] != argv[argc]) {
			fp = fopen(argv[i], "r");
			if (fp == NULL) {
				printf("File %s has failed to open", argv[i]);
			}
			else {
				concatenate(&fp, &lc, &argv[1]);
			}
			i++;
		}
	}
	else {
		while (argv[i] != argv[argc]) {
			fp = fopen(argv[i], "r");
			if (fp == NULL) {
				printf("File %s has failed to open", argv[i]);
			}
			else {
				concatenate(&fp, &lc, &argv[1]);
			}
			i++;
		}
	}
}