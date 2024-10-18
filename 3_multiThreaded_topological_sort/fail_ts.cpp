#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <cassert>
#include <time.h>
#include <fstream>
#include "env.cpp"

using namespace std;

using llong = long long int;

const llong num_roots = get_int_env("NUM_ROOTS", 4);
const llong  depth    = get_int_env("GRAPH_DEPTH", 10);
std::vector<std::vector<llong>> graph;
std::vector<llong> visited;
std::vector<llong> top_stack;  // topologically ordered stack
std::vector<llong> position;   // position of each vertex in the topological order
ofstream* myfile;              // program writes the topological order in this file
void explore_children(llong vertex);

void runDFS() {
	visited.assign(num_roots*depth, 0);
	top_stack.clear();
	
	for (llong vertex = 0; vertex<num_roots*depth; vertex++) {
		if (visited[vertex] == 0) {
			explore_children(vertex);
		}
	}
return;
}

void explore_children(llong vertex) {
	visited[vertex] = 1;
	for (int i = 0; i<10000; i++) {}	
	for (llong child: graph[vertex]) {
		if (visited[child] == 0) {
			explore_children(child);
		}
	}
	top_stack.push_back(vertex);
	position[vertex] = top_stack.size()-1;
	if (num_roots*depth < 1000) {            // if small number of write write in iostream
		std::cout<<vertex<<" ";
	}
	(*myfile) << vertex << " ";
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

	runDFS();
	verify_order();	
	(*myfile).close(); delete myfile;

	time(&end); double dif = difftime (end,start);
        std::cout<<"total elapsed time (regular run): "<<dif<<"\n";
        std::cout<<"the ordering is printed in file: top_sort.txt"<<"\n\n";
return 0;
}
