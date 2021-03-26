#Variables
cpu.c:=./src/cpu.c
cpu.o:=./src/cpu.o
check_cpu.c:=./tests/check_cpu.c
check_cpu.o:=./tests/check_cpu.o

Test: $(cpu.o) $(check_cpu.o)
	gcc -g $(cpu.o) $(check_cpu.o) -lcheck -lm -lpthread -lrt -o ./tests/check_cpu
	./tests/check_cpu
cpu.o: $(cpu.c)
	gcc -g -c $(cpu.c)

check_cpu.o: $(check_cpu.c)
	gcc -g -c $(check_cpu.c)
