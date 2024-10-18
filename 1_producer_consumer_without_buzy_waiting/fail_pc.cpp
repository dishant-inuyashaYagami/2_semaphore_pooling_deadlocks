#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int limit = 10;
int num_iter = 1000;
int counter = 0;

void* producer(void* empty) {
	while (counter >= limit) {    // buffer already full
	}
	// ----- produces something ------
	counter++;
return NULL;
}

void* consumer(void* empty) {
	while (counter <= 0) {    // buffer is empty
	}
	// ------ counsumes something ------
	counter--;
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
	std::cout<<"\nhello hackerman!... failed solution; stuck in a loop; example sequence: pcpc; where counter: (p=1) (c=0 p=2)->0 (c=loop) "<<std::endl;

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
