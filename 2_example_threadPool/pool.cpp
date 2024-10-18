#include <iostream>
#include <list>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int num_threads = 32;
const int num_iter    = 20000;

void regular();      // non-multithreaded main call
void task();	     // it is called multiple times

bool task_remaining = true;
std::list<int> queue;
sem_t sem_pool_access;         // only ine thread is allowed to access the pool at a time
sem_t sem_consumer;
sem_t sem_producer;
sem_t sem_access_semc;        // mutex for accessing the value of sem_consumer
int num_task_executed = 0; 

// each thread run this // waits for the task in the queue to be available
void* pool_run(void* empty) {
	int value;
	while(true) {
		sem_wait(&sem_access_semc);             // used to terminate the threads
		sem_getvalue(&sem_consumer, &value);
		if (!task_remaining && value == 0) {
			sem_post(&sem_access_semc);
			break;
		}
		sem_wait(&sem_consumer);
		sem_post(&sem_access_semc);

		sem_wait(&sem_pool_access);
		queue.pop_front();
		num_task_executed++;
		sem_post(&sem_pool_access);
		sem_post(&sem_producer);

		task(); // running the task
	}

	
}

int main() {
	// regular run
	regular();
	
	// thread pool run
	time_t start,end; time(&start);	
	sem_init(&sem_pool_access, 0 , 1);
	sem_init(&sem_access_semc, 0 , 1);
	sem_init(&sem_consumer, 0 , 0);
	sem_init(&sem_producer, 0 , num_threads);

	pthread_t tid[num_threads];
        pthread_attr_t attr; pthread_attr_init(&attr);
	for(int t = 0; t<num_threads; t++) {
                pthread_create(&tid[t], &attr, pool_run, NULL);
        }
	std::cout<<"\nthreads created!"<<std::endl;

	for (int i = 0; i<num_iter; i++) {  // task producer
		sem_wait(&sem_producer);
		sem_wait(&sem_pool_access);
                queue.push_back(1);
		sem_post(&sem_pool_access);
		sem_post(&sem_consumer);
        }
	task_remaining = false;		

	for(int t = 0; t<num_threads; t++) {
                pthread_join(tid[t],NULL);
        }
        std::cout<<"thread joined\n";
        std::cout<<"number of tasks executed: "<<num_task_executed<<"\n";
	
	time(&end); double dif = difftime (end,start);
	std::cout<<"\nelapsed time of thread pool run: "<<dif<<"\n";
}


// =============================================
void regular() {
	time_t start,end; time(&start);
	for (int i = 0; i<num_iter; i++) {
		task();
	}	

	time(&end); double dif = difftime (end,start);
	std::cout<<"\nelapsed time of regular run: "<<dif<<"\n";
}

void task() {
	for (int i = 0;i <= 1000000; i++) {
	}
return;
}

