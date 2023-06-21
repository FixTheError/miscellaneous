
# include <stdio.h>

void getwc(FILE** fp, int* tw, int* tl, int* tc) {
	char Message_From, Harry_Manback;    //a real character judging by the voicemail recording
	int lc = 0;
	int wc = 0;
	int cc = 0;
	while ((Harry_Manback = getc(*fp)) != EOF) {
		cc++;
		if ((Harry_Manback == ' ' || Harry_Manback == '\n') && Message_From != ' ' && Message_From != '\n') {
			wc++;
		}
		if (Harry_Manback == '\n') {
			lc++;
		}
		Message_From = Harry_Manback;
	} // a little help from https://www.opentechguides.com/how-to/article/c/72/c-file-counts.html
	printf("%d	", lc);
	printf("%d	", wc);
	printf("%d	", cc);
	*tl += lc;
	*tw += wc;
	*tc += cc;
}

int main(int argc, char *argv[]) {
	FILE *fp;
	int tc = 0;
	int tw = 0;
	int tl = 0;
	int i = 1;
	if (argc > 1) {
		while (argv[i] != argv[argc]) {
			fp = fopen(argv[i], "r");
			getwc(&fp, &tw, &tl, &tc);
			int a = 0;
			printf("%s\n", argv[i]);
			i++;
		}
		printf("%d	", tl);
		printf("%d	", tw);
		printf("%d	", tc);
		printf("total\n");
	}
	else {
		fp = stdin;
		printf("\n");
		getwc(&fp, &tw, &tl, &tc);
		printf("\n");
	}
	return 0;
}