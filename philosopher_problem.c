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
//Determine the state of an adjacent philosopher.
void test(int i) {
	int left = (i + N - 1) % N;
	int right = (i + 1) % N;
	//If neither adjacent philosopher is eating and the one in the patrameter is hungry, change state to eating.
	if (statearray[i] == HUNGRY && statearray[left] != EATING && statearray[right] != EATING) {
		statearray[i] = EATING;
		printf("philosopher %d is now eating\n", i);
		sem_post(&s[i]);
	}
}
//Philosopher is done eating, put forks down and let another eat
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
//Change the philosopher state to hungry and let them eat if forks are available.
void takeforks(int i) {
	sem_wait(&mutex);
	statearray[i] = HUNGRY;
	printf("philosopher %d is now hungry\n", i);
	test(i);
	sem_post(&mutex);
	sem_wait(&s[i]);
}
//Each philosopher cycles through states 3 times.
//Function accepts a void pointer that points to the index number of the philosopher.
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
