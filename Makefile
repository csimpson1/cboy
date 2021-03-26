#Variables
cpu.c:=./src/cpu.c
cpu.o:=./src/cpu.o

check_cpu.c:=./tests/check_cpu.c
check_cpu.o:=./tests/check_cpu.o

bit_fucntions.c:=./src/bit_functions.c
bit_functions.o:=./src/bit_functions.o

Test: $(cpu.o) $(check_cpu.o) $(bit_functions.o)
	gcc -g $(cpu.o) $(check_cpu.o) $(bit_functions.o)  -lcheck -lm -lpthread -lrt -o ./tests/check_cpu
	./tests/check_cpu
cpu.o: $(cpu.c)
	gcc -g -c $(cpu.c)

check_cpu.o: $(check_cpu.c)
	gcc -g -c $(check_cpu.c)

bit_functions.o: $(bit_functions.c)
	gcc -g -c $(bit_functions.c)
