#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int limit = 10;
int num_iter = 1000;
int counter = 0;
sem_t sem_p;
sem_t sem_c;
sem_t mutex;

void* producer(void* empty) {
	sem_wait(&sem_p);    // wait for empty space in buffer
	sem_wait(&mutex);
	// ----- produces something ------
	counter++;
	sem_post(&mutex);
	sem_post(&sem_c);    // signal the consumers to consume
return NULL;
}

void* consumer(void* empty) {
	sem_wait(&sem_c);            // ensures that counter is >=0 before consuming, else loop
	sem_wait(&mutex);
	// ------ counsumes something ------
	counter--;
	sem_post(&mutex);
	sem_post(&sem_p);
return NULL;
}

void* all_p(void *empty) {                          // series of produer commands
	for (int i = 0; i <= num_iter; i++) {
		producer(empty);
	}
return NULL;
}

void* all_c(void *empty) {                          // series of consumer commands
	for (int i = 0; i <= num_iter; i++) {
		consumer(empty);
	}
return NULL;
}

int main() {
	std::cout<<"\nhello hackerman!... the solution is using 3 semaphore; most efficient since no buzy waiting"<<std::endl;
	sem_init(&sem_p, 0 , limit);
	sem_init(&sem_c, 0 , 0);
	sem_init(&mutex, 0 , 1);

	pthread_t pt;
	pthread_t pc;
	pthread_attr_t attr; pthread_attr_init(&attr);

	pthread_create(&pc,  &attr, all_c, NULL);
	pthread_create(&pt,  &attr, all_p, NULL);
	
	pthread_join(pc,  NULL);
	pthread_join(pt,  NULL);
	std::cout<<"; threads joined!"<<std::endl;
	std::cout<<"number of elements remaining: "<<counter<<"\n\n"<<std::endl;
return 0;
}
