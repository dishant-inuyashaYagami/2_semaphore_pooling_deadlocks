#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int limit = 10;
int num_iter = 1000;
int counter = 0;
sem_t mutex;

void* producer(void* empty) {
	while (counter >= limit) {    // buffer already full
	}
	// ----- produces something ------
	sem_wait(&mutex);
	counter++;
	sem_post(&mutex);
return NULL;
}

void* consumer(void* empty) {
	while (counter <= 0) {    // buffer is empty
	}
	// ------ counsumes something ------
	sem_wait(&mutex);
	counter--;
	sem_post(&mutex);
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
	std::cout<<"\nhello hackerman!... not an efficient solution since uses buzy waiting"<<std::endl;
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
