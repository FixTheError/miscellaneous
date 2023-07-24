#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//header struct contains the size and name of the input file.
struct header {
	char name[256];
	int size;
};

int main(int argc, char* argv[]) {
	//Set up necessary variables.
	int a = 0;
	int l = 0;
	int e = 0;
	int opt;
	int numopts = 0;
	int found = 0;
	FILE* archive;
	FILE* file;
	char* buffer;
	//get command line options and set flags based on which options were included.
	while ((opt = getopt(argc, argv, "ale")) != -1) {
		switch (opt) {
		case 'a':
			a = 1;
			numopts++;
			break;
		case 'l':
			l = 1;
			numopts++;
			break;
		case 'e':
			e = 1;
			numopts++;
			break;
		default:
			printf("Invalid option, exiting.");
			return 0;
		}
	}
	archive = fopen(argv[numopts + 1], "a+");
	if (a == 1) {
		file = fopen(argv[numopts + 2], "r");
		struct header* head = malloc (sizeof(struct header));
		strcpy(head->name, argv[numopts + 2]);
		fseek(file, 0, SEEK_END);
		head->size = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer = malloc(head->size);
		fwrite(head, sizeof(struct header), 1, archive);
		fread(buffer, sizeof(char), (head->size/sizeof(char)), file);
		fwrite(buffer, sizeof(char), (head->size / sizeof(char)), archive);
	}
	if (l == 1) {
		int offset = 0;
		struct header* head = malloc (sizeof(struct header));
		fseek(archive, 0, SEEK_SET);
		while (1) {
			fread(head, sizeof(struct header), 1, archive);
			if (feof(archive)) {
				break;
			}
			printf("name: %s, size: %d Bytes\n", head->name, head->size);
			offset = head->size;
			fseek(archive, offset, SEEK_CUR);
		}
	}
	if (e == 1) {
		int offset = 0;
		struct header* head = malloc (sizeof(struct header));
		fseek(archive, 0, SEEK_SET);
		while (1) {
			fread(head, sizeof(struct header), 1, archive);
			if (feof(archive)) {
				break;
			}
			if (strcmp(head->name, argv[argc - 1]) == 0) {
				file = fopen(head->name, "w");
				buffer = malloc(head->size);
				fread(buffer, sizeof(char), (head->size / sizeof(char)), archive);
				fwrite(buffer, sizeof(char), (head->size / sizeof(char)), file);
				found = 1;
				break;
			}
			offset = head->size;
			fseek(archive, offset, SEEK_CUR);
		}
		if (found == 0) {
			printf("%s was not found in the specified archive\n", argv[argc - 1]);
		}
	}
}
