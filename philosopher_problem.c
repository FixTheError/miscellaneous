#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int N;
int* statearray;
sem_t mutex;
sem_t* s;

void test(int i) {
	int left = (i + N - 1) % N;
	int right = (i + 1) % N;
	if (statearray[i] == HUNGRY && statearray[left] != EATING && statearray[right] != EATING) {
		statearray[i] = EATING;
		printf("philosopher %d is now eating\n", i);
		sem_post(&s[i]);
	}
}

void putforks(int i) {
	int left = ((i + N - 1) % N);
	int right = ((i + 1) % N);
	sem_wait(&mutex);
	statearray[i] = THINKING;
	printf("philosopher %d is now thinking\n", i);
	test(left);
	test(right);
	sem_post(&mutex);
}

void takeforks(int i) {
	sem_wait(&mutex);
	statearray[i] = HUNGRY;
	printf("philosopher %d is now hungry\n", i);
	test(i);
	sem_post(&mutex);
	sem_wait(&s[i]);
}

void* philosopher(void* p) {
	int k = (int)p;
	for (int j = 0; j < 3; j++) {
		takeforks(k);
		putforks(k);
	}

}

int main(int argc, char* argv[]) {
	//Get the number of philosophers from the command line args and allocate an array for philosopher states, one for semaphores, and one for threads.
	N = atoi(argv[1]);
	statearray = malloc(N * sizeof(int));
	s = malloc(N * sizeof(sem_t));
	sem_init(&mutex, 0, 1);
	pthread_t threads[N];
	//Initialize all the semaphores.
	for (int j = 0; j < N; j++) {
		sem_init(&s[j], 0, 0);
	}
	//Create a new thread for each philosopher.
	for (int j = 0; j < N; j++) {
		pthread_create(&threads[j], NULL, philosopher, j);
		printf("philosopher %d created\n", j);
	}
	//Collect all the threads before exiting.
	for (int j = 0; j < N; j++) {
		pthread_join(threads[j], NULL);
	}
}
