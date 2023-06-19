#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int length = 5;
int heapsize;

void insertion_sort(int* arr);
void merge_sort(int* arr, int p, int r);
void merge(int* arr, int p, int q, int r);
int parent(int i);
int left(int i);
int right(int i);
void max_heapify(int* arr, int i);
void build_max_heap(int* arr);
void heapsort(int* arr);

void main() {
	int* arr = malloc(sizeof(int) * length);
	int j = length;
	for (int i = 0; i < length; i++) {
		arr[i] = j;
		j--;
	}
	for (int k = 0; k < length; k++) {
		printf("%d\n", arr[k]);
	}
	insertion_sort(arr);
	for (int k = 0; k < length; k++) {
		printf("%d\n", arr[k]);
	}
}

void insertion_sort(int* arr) {
	for (int j = 1; j < length; j++) {
	int key = arr[j];
	int i = j - 1;
	while (i >= 0 && arr[i] > key) {
		arr[i + 1] = arr[i];
		i = i - 1;
	}
	arr[i + 1] = key;
	}
	for (int k = 0; k < length; k++) {
		printf("%d\n", arr[k]);
	}
}

void merge_sort(int* arr, int p, int r) {
	if (p < r) {
		int q = (p + (r - 1)) / 2;
		merge_sort(arr, p, q);
		merge_sort(arr, q + 1, r);
		merge(arr, p, q, r);
	}
}

void merge(int* arr, int p, int q, int r) {
	int num1 = q - p + 1;
	int num2 = r - q;
	int L[num1 + 1];
	int R[num2 + 1];
	for (int i = 0; i < num1; i++) {
		L[i] = arr[p + i];
	}
	for (int j = 0; j < num2; j++) {
		R[j] = arr[q + j /*+ 1*/];
	}
	L[num1] = INT_MAX;
	R[num2] = INT_MAX;
	int i = 0;
	int j = 0;
	for (int k = p; k <= r; k++) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i = i + 1;
		}
		else {
			arr[k] = R[j];
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

void max_heapify(int* arr, int i) {
	int largest;
	int l = left(i);
	int r = right(i);
	if (l < heapsize && arr[l] > arr[i]) {
		largest = l;
	}
	else {
		largest = i;
	}
	if (r < heapsize && arr[r] > arr[largest]) {
		largest = r;
	}
	if (largest != i) {
		int temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
		max_heapify(arr, largest);
	}
}

void build_max_heap(int* arr) {
	heapsize = length;
	for (int i = (heapsize / 2) - 1; i > 0; i--) {
		max_heapify(arr, i);
	}
}

void heapsort(int* arr) {
	build_max_heap(arr);
	for (int i = length - 1; i > 0; i--) {
		int temp = arr[i];
		arr[i] = arr[0];
		arr[0] = temp;
		heapsize = heapsize - 1;
		max_heapify(arr, 0);
	}
}