all: fail.out pass.out
	./fail.out
	./pass.out

fail.out: fail.cpp
	g++ --std=c++11 -lpthread fail.cpp -o fail.out

pass.out: pass.cpp
	g++ --std=c++11 -lpthread pass.cpp -o pass.out
