rom = register_check

gbrom: $(rom).o
	rgblink -o $(rom).gb $(rom).o
	#Fix the header file
	rgbfix -v -p 0 $(rom).gb	

$(rom).o: $(rom).asm
	rgbasm -o $(rom).o $(rom).asm

clean:
	rm $(rom).gb $(rom).o
