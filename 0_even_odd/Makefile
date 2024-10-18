all: pass.out fail.out
	./pass.out
	./fail.out

pass.out: pass_even_odd.cpp
	g++ pass_even_odd.cpp -lpthread -o pass.out


fail.out: fail_even_odd.cpp
	g++ fail_even_odd.cpp -lpthread -o fail.out
