#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <cassert>
#include <time.h>
#include <fstream>
#include "env.cpp"

#include <list>
#include <pthread.h>
#include <semaphore.h>

#include <algorithm> 
#include <random>     // for suffling the set of vertices to ve traversed by DFS

using namespace std;

using llong = long long int;

//-------------------------------- regular apis ---------------------------
const llong num_roots = get_int_env("NUM_ROOTS", 4);
const llong  depth    = get_int_env("GRAPH_DEPTH", 10);
std::vector<std::vector<llong>> graph;
std::vector<llong> visited;
std::vector<llong> top_stack;  // topologically ordered stack
std::vector<llong> position;   // position of each vertex in the topological order
ofstream* myfile;              // program writes the topological order in this file
void explore_children(llong vertex);
// ------------------------------------------------------------------------


// ------------------------------- multi-threded apis -----------------------
const int num_threads = 32;
sem_t sem_producer;
sem_t sem_consumer, sem_c_access;
sem_t sem_q_access;
sem_t* sem_vertex;                    // a mutex for each vertex
sem_t sem_stack_access;               // mutex for topological stack
void init_sem();                      // initializes the semaphore
bool task_remaining = true;
std::list<llong> queue;               // task queue

void* pool_run (void* empty) {
	while (true) {
		int value;
		sem_wait(&sem_c_access);
		sem_getvalue(&sem_consumer, &value);
		if (!task_remaining && value == 0) {
			sem_post(&sem_c_access);
			break;
		}
		sem_wait(&sem_consumer);
		sem_post(&sem_c_access);
		sem_wait(&sem_q_access);
		llong vertex = queue.front();
		queue.pop_front();
		sem_post(&sem_q_access);
		sem_post(&sem_producer);

		explore_children(vertex);   // running the task
	}
}
// --------------------------------------------------------------------------

void runDFS() {
	visited.assign(num_roots*depth, 0);
	top_stack.clear();
	init_sem();
	pthread_t tid[num_threads];
        pthread_attr_t attr; pthread_attr_init(&attr);
        for(int t = 0; t<num_threads; t++) {
                pthread_create(&tid[t], &attr, pool_run, NULL);
        }
        std::cout<<"\nthreads created!"<<std::endl;
	
	std::vector<llong> all_vertices;
	for (llong vertex = 0; vertex<num_roots*depth; vertex++) {   // suffle the vertices before traversal
		all_vertices.push_back(vertex);	
	}
	auto rng = std::default_random_engine {};
	std::shuffle(std::begin(all_vertices), std::end(all_vertices), rng);

	for (llong vertex: all_vertices) {  // producer of task: explore_children
		sem_wait(&sem_producer);
		sem_wait(&sem_q_access);
		queue.push_back(vertex); 
		sem_post(&sem_q_access);
		sem_post(&sem_consumer);
		//vertex += depth-1;
	}
	task_remaining = false;

	for(int t = 0; t<num_threads; t++) {
                pthread_join(tid[t],NULL);
        }
        std::cout<<"\nthread joined\n";
return;
}

void explore_children(llong vertex) {
	sem_wait(&sem_vertex[vertex]);
	if (visited[vertex] != 0) {
		sem_post(&sem_vertex[vertex]);
		return;
	}
	visited[vertex] = 1;
	sem_post(&sem_vertex[vertex]);
	
	for (int i = 0; i<10000; i++) {}	
	
	for (llong child: graph[vertex]) {
		explore_children(child);
		while (visited[child]!=2) {}
	}
	
	sem_wait(&sem_stack_access);
	top_stack.push_back(vertex);
	position[vertex] = top_stack.size()-1;
	if (num_roots*depth < 1000) {            // if small number of write write in iostream
		std::cout<<vertex<<" ";
	}
	(*myfile) << vertex << " ";
	sem_post(&sem_stack_access);

	visited[vertex] = 2;          // completed its subtree  // we do not need semphore here since no two threads will come at this state
return;
}

// verifies if the obtained order of vertices is indeed toplogically sorted
void verify_order() {
	for (llong vertex = 0; vertex < num_roots*depth; vertex++) {
		llong from = vertex;
		for (llong to: graph[vertex]) {
			assert(position[to] >= 0 && position[from] >= 0);
			if (position[to] >= position[from]) {
				std::cout<<"\n\nIncorrect Topological Ordering!"<<std::endl;
				std::cout<<"To: "<<to<<", From: "<<from<<"\n"<<std::endl;
				return;
			}
		}
	}
	std::cout<<"\n\nCorrect Topological Ordering!\n"<<std::endl;
}

int main() {
	time_t start,end; time(&start);
	myfile = new ofstream("top_sort.txt");		

	std::cout<<"welcome hackerman!"<<std::endl;
	
	// creating DAG
	graph.assign(num_roots*depth, std::vector<llong>());
	position.assign(num_roots*depth, -1);    
	llong  num_vertices = 0;
	for (llong i = 0; i< num_roots; i++) {
		for (llong j = 0; j< depth-1; j++) {
			graph[i*depth+j].push_back(i*depth + j + 1);
		}
	}
	
	for (llong i = 0; i< num_roots; i++) {
		graph[i*depth + depth/2].push_back(((i+1)%num_roots)*depth + depth/2 + 1);
		graph[i*depth + depth/2].push_back(((num_roots + i - 1)%num_roots)*depth + depth/2 + 1);
	}

	runDFS();    // uses thread
	verify_order();	
	(*myfile).close(); delete myfile;

	time(&end); double dif = difftime (end,start);
        std::cout<<"total elapsed time (multi-threaded): "<<dif<<"\n";
        std::cout<<"the ordering is printed in file: top_sort.txt"<<"\n";
        std::cout<<"the time can be further improved if you mark the root vertices and start traversal from there! (hint: vertex += depth)"<<"\n";
	std::cout<<"note that you might see time degradation if you are simply traversing the vertices"<<std::endl;
	std::cout<<"this time difference is observed since we are performing ~ 1000 operations at each vertex"<<std::endl<<std::endl;
return 0;
}

// ------------------- initilizes the semaphores ------------
void init_sem() {
	sem_vertex = new sem_t[num_roots*depth];
	for (llong i = 0; i < num_roots*depth; i++) {
		sem_init(&sem_vertex[i], 0, 1);
	}
	sem_init(&sem_consumer, 0, 0); sem_init(&sem_c_access, 0, 1);
	sem_init(&sem_producer, 0, num_threads);
	sem_init(&sem_q_access, 0, 1);
	sem_init(&sem_stack_access, 0, 1);
}
