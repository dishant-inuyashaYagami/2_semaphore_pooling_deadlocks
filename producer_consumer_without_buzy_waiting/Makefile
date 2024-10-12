all: pass.out fail.out only_mutex.out
	./pass.out
	./only_mutex.out
	./fail.out

pass.out: pass_pc.cpp
	g++ pass_pc.cpp -lpthread -o pass.out

fail.out: fail_pc.cpp
	g++ fail_pc.cpp -lpthread -o fail.out

only_mutex.out: using_only_mutex.cpp
	g++ using_only_mutex.cpp -lpthread -o only_mutex.out
