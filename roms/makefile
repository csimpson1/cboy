gbrom: hello-world.o
	rgblink -o hello-world.gb hello-world.o
	#Fix the header file
	rgbfix -v -p 0 hello-world.gb	

hello-world.o: hello-world.asm
	rgbasm -o hello-world.o hello-world.asm

clean:
	rm hello-world.gb hello-world.o
