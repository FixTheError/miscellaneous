#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

int N;
int* buf;
int ccount = 0;
int pcount = 0;
sem_t cons;
sem_t prod;
sem_t mutex;

void* producer() {
	int item = pcount;
	while (1) {
		item = pcount;
		sem_wait(&cons);
		sem_wait(&mutex);
		buf[pcount] = item;
		printf("%d has been added\n", item);
		pcount = (pcount + 1)%N;
		if (pcount == ccount) {
			printf("buffer is full\n");
		}
		sem_post(&mutex);
		sem_post(&prod);
	}
}

void* consumer() {
	int item;
	while (1) {
		sem_wait(&prod);
		sem_wait(&mutex);
		item = buf[ccount];
		printf("%d has been removed\n", item);
		ccount = (ccount + 1) % N;
		if (ccount == pcount) {
			printf("buffer is empty\n");
		}
		sem_post(&mutex);
		sem_post(&cons);
		printf("item consumed: %d\n", item);
	}
}

int main(int argc, char* argv[]) {
	int consumers = 1;
	int producers = 1;
	N = 10;
	switch (argc) {
	case 2:
		producers = atoi(argv[1]);
		break;
	case 3:
		producers = atoi(argv[1]);
		consumers = atoi(argv[2]);
		break;
	case 4:
		producers = atoi(argv[1]);
		consumers = atoi(argv[2]);
		N = atoi(argv[3]);
		break;
	default:
		break;
	}
	sem_init(&cons, 0, N);
	sem_init(&prod, 0, 0);
	sem_init(&mutex, 0, 1);
	buf = malloc(N * sizeof(int));
	pthread_t prodthreads[producers];
	pthread_t consthreads[consumers];
	for (int i = 0; i < producers; i++) {
		pthread_create(&prodthreads[i], NULL, producer, NULL);
	}
	for (int j = 0; j < consumers; j++) {
		pthread_create(&consthreads[j], NULL, consumer, NULL);
	}
	for (int k = 0; k < producers; k++) {
		pthread_join(prodthreads[k], NULL);
	}
	for (int l = 0; l < producers; l++) {
		pthread_join(prodthreads[l], NULL);
	}
}