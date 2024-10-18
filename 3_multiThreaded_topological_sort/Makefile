all: fail.out pass.out
	./fail.out
	./pass.out

fail.out: fail_ts.cpp
	g++ --std=c++11 fail_ts.cpp -lpthread -o fail.out
	#g++ --std=c++11 -g fail_ts.cpp -lpthread -o fail.out
 
pass.out: pass_ts.cpp
	g++ --std=c++11 pass_ts.cpp -lpthread -o pass.out 
	#g++ --std=c++11 -g pass_ts.cpp -lpthread -o pass.out 
