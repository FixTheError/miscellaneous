#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int length = 5;
int heapsize;
int max_len = 0;
int wc = 0;

void insertion_sort(char** arr);
void merge(char** arr, int p, int q, int r);
void merge_sort(char** arr, int p, int r);
int parent(int i);
int left(int i);
int right(int i);
void max_heapify(char** arr, int i);
void build_max_heap(char** arr);
void heapsort(char** arr);

void main() {
	FILE* fs;
	char* buf = NULL;
	char** arr = malloc(sizeof(char*) * length);
	char* fn = "testfile.txt";
	size_t bufsize = 0;
	int i = 0;
	int temp;
	fs = fopen(fn, "r");
	if (fs == NULL) {
		printf("Error: unable to open %s\n", fn);
	}
	while ((temp = getline(&buf, &bufsize, fs)) != -1) {
		if (i == length) {
			length = length + 5;
			arr = (char**)realloc(arr, length * sizeof(char*));
			if (arr == NULL) {
				printf("Error: memory leak");
			}
		}
		if (temp > max_len) {
			max_len = temp;
		}
		arr[i] = malloc((temp + 1) * sizeof(char));
		strcpy(arr[i], buf);
		i++;
		wc++;
	}
	free(buf);
	for (int j = 0; j < i; j++) {
		printf("%s\n", arr[j]);
	}
	merge_sort(arr, 0, wc-1);
	for (int j = 0; j < i; j++) {
		printf("%s\n", arr[j]);
	}
	for (int k = 0; k < length; k++) {
		free(arr[k]);
	}
	free(arr);
	free(fs);
	exit(EXIT_SUCCESS);
}

void insertion_sort(char** arr) {
	for (int j = 1; j < (wc - 1); j++) {
	char* key = malloc((strlen(arr[j]) + 1) * sizeof(char));
	strcpy(key, arr[j]);
	int i = j - 1;
	while (i >= 0 && strcmp(key, arr[i]) < 0) {
		strcpy(arr[i + 1], arr[i]);
		i = i - 1;
	}
	strcpy(arr[i + 1], key);
	free(key);
	}
}

void merge_sort(char** arr, int p, int r) {
	if (p < r) {
		int q = ((p + r) / 2);
		merge_sort(arr, p, q);
		merge_sort(arr, q + 1, r);
		merge(arr, p, q, r);
	}
}

void merge(char** arr, int p, int q, int r) {
	int num1 = q - p + 1;
	int num2 = r - q;
	char** L = malloc((num1 * sizeof(char*)) + (max_len * sizeof(CHAR_MAX)));
	char** R = malloc((num2 * sizeof(char*)) + (max_len * sizeof(CHAR_MAX)));
	char* strtemp = malloc(max_len * sizeof(char));
	for (int s = 0; s < max_len; s++) {
		strtemp[s] = CHAR_MAX;
	}
	for (int i = 0; i < num1; i++) {
		L[i] = malloc((strlen(arr[p + i]) + 1) * sizeof(char));
		strcpy(L[i], arr[p + i]);
	}
	for (int j = 0; j < num2; j++) {
		R[j] = malloc((strlen(arr[q + j + 1]) + 1) * sizeof(char));
		strcpy(R[j], arr[q + j + 1]);
	}
	L[num1] = malloc(max_len * sizeof(CHAR_MAX));
	R[num2] = malloc(max_len * sizeof(CHAR_MAX));
	strcpy(L[num1], strtemp);
	strcpy(R[num2], strtemp);
	int i = 0;
	int j = 0;
	for (int k = p; k <= r; k++) {
		if (strcmp(L[i], R[j]) <= 0) {
			strcpy(arr[k], L[i]);
			i = i + 1;
		}
		else {
			strcpy(arr[k], R[j]);
			j = j + 1;
		}
	}
}

int parent(int i) {
	i = ((i - 1) / 2);
	return i;
}

int left(int i) {
	i = (2 * i) + 1;
	return i;
}

int right(int i) {
	i = (2 * i) + 2;
	return i;
}

void max_heapify(char** arr, int i) {
	int largest;
	int l = left(i);
	int r = right(i);
	if (l < heapsize && strcmp(arr[l], arr[i]) > 0) {
		largest = l;
	}
	else {
		largest = i;
	}
	if (r < heapsize && strcmp(arr[r], arr[largest]) > 0) {
		largest = r;
	}
	if (largest != i) {
		char* temp = malloc((strlen(arr[i]) + 1) * sizeof(char));
		strcpy(temp, arr[i]);
		strcpy(arr[i], arr[largest]);
		strcpy(arr[largest], temp);
		max_heapify(arr, largest);
	}
}

void build_max_heap(char** arr) {
	heapsize = wc;
	for (int i = (heapsize / 2) - 1; i >= 0; i--) {
		max_heapify(arr, i);
	}
}

void heapsort(char** arr) {
	build_max_heap(arr);
	for (int i = wc - 1; i >= 0; i--) {
		char* temp = malloc((strlen(arr[i]) + 1) * sizeof(char));
		strcpy(temp, arr[i]);
		strcpy(arr[i], arr[0]);
		strcpy(arr[0], temp);
		heapsize = heapsize - 1;
		max_heapify(arr, 0);
	}
}