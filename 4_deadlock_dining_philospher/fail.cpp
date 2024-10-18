#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <bits/stdc++.h>

#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;

const int work_time = 50000;     // milliseconds (~10 sec)
const int num_threads = 20;      // number of philophers
sem_t sem_stick[num_threads];    // mutex for each chopstick
int   acq_by_phil[num_threads];  // chopstick --> acquired by the philosopher
void* think_eat(void* thread_id);
void acquire(int stick, int phil);
void release(int stick);
void print_stick_belonging();
void print(int, string);
sem_t sem_print;

bool parent_calling = false;

int main() {
	std::cout<<"hello philosophers..."<<std::endl;
	
	sem_init(&sem_print, 0, 1);
	for (int i = 0; i<num_threads; i++) {sem_init(&sem_stick[i], 0 ,1); acq_by_phil[i] = -1;}   // initializes the semaphore for each chopstick

	pthread_t tid[num_threads];
        pthread_attr_t attr; pthread_attr_init(&attr);
	int* id = new int[num_threads];
        for(int t = 0; t<num_threads; t++) {
		id[t] = t; 
                pthread_create(&tid[t], &attr, think_eat, &id[t]);
        }
        std::cout<<"\nthreads created! philosophers thinking..."<<std::endl;
	
	std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
	parent_calling = true;

	for(int t = 0; t<num_threads; t++) {
                pthread_join(tid[t],NULL);
        }
        std::cout<<"\nthread joined! times up!\n\n";
return 0;
}


// philosopher eating and thinking
void* think_eat(void* thread_id) {
	int phil_id = *((int*) thread_id);
	//std::cout << "philosopher " << phil_id << " working..."<<std::endl;
	while (!parent_calling) {
		int think = 100 + rand()%100;				
		print(phil_id, "thinking");  // thinking
		usleep(think);
	
		int eat = 100 + rand()%100;		
		print(phil_id, "waiting"); // waiting
		acquire(phil_id%num_threads, phil_id);		// waiting till both the chopsticks are available
		acquire((phil_id+1)%num_threads, phil_id);
		
		print(phil_id, "eating");  // eating
		usleep(eat);		
	
		release(phil_id%num_threads);		        // releasing the chopsticks after eating
		release((phil_id+1)%num_threads);		
	}		
}

// acquire chopstick
void acquire(int stick, int phil) {
	sem_wait(&sem_stick[stick]);
	acq_by_phil[stick] = phil;
}

// relese chopstick
void release(int stick) {
	acq_by_phil[stick] = -1;
	sem_post(&sem_stick[stick]);
}

void print(int phil_id, string str) {
	sem_wait(&sem_print);
	std::cout<<"phil "<<phil_id<<" "<<str<<"\n";
	if (str == "waiting") {
		print_stick_belonging();
	}
	sem_post(&sem_print);
}

void print_stick_belonging () {
	int same_assigned_count = 0;
	for (int i = 0; i< num_threads; i++) {std::cout<<"[stick "<<i<<", phil "<< acq_by_phil[i] <<"]  ";
		 if (i == acq_by_phil[i]) { same_assigned_count++;}
	}
	std::cout<<"\n";
	if (same_assigned_count >= num_threads - 1) {
		std::cout<<"\n     ===========  DEADLOCK HAPPENED! =========  \n";
		string x; std::cout << "press enter to exit... " <<std::endl; std::cin.ignore();
		exit(0);
	}
}
