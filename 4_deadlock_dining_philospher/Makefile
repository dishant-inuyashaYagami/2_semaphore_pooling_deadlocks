all: fail.out pass.out pass_two.out
	./fail.out
	./pass.out
	./pass_two.out

fail.out: fail.cpp
	g++ --std=c++11 -lpthread fail.cpp -o fail.out

pass.out: pass.cpp
	g++ --std=c++11 -lpthread pass.cpp -o pass.out

pass_two.out: pass_two.cpp
	g++ --std=c++11 -lpthread pass_two.cpp -o pass_two.out
