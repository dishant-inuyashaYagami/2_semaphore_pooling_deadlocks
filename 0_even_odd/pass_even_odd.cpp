#include<iostream>
#include<pthread.h>
#include<semaphore.h>

using namespace std;

sem_t sem_even;
sem_t sem_odd;

void* print_even(void* empty) {
	for(int i = 0; i<100; i++) {
		if (i%2==0) {
			sem_wait(&sem_even);
			std::cout<<i<<" ";
			sem_post(&sem_odd);
		}
	}
}

void* print_odd(void* empty) {
        for(int i = 0; i<100; i++) {
                if (i%2==1) {
			sem_wait(&sem_odd);
                        std::cout<<i<<" ";
			sem_post(&sem_even);
                }
        }
}


int main () {
	std::cout<<"hello hackerman!"<<std::endl;
	sem_init(&sem_even, 0 , 1);
	sem_init(&sem_odd, 0 , 0);	
	pthread_t tid_even, tid_odd;
        pthread_attr_t attr; pthread_attr_init(&attr);
	
	pthread_create(&tid_even, &attr, print_even, NULL);
	pthread_create(&tid_odd,  &attr, print_odd,  NULL);
	
	pthread_join(tid_even, NULL);
	pthread_join(tid_odd,  NULL);
	std::cout<<"; threads joined!"<<std::endl;
return 0;
}
