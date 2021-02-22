
#include <assert.h>
#include <stdio.h>
#include "cpu.h"


void test_get_high_byte();
void test_get_low_byte();
void test_set_low_byte();
void test_set_high_byte();
void test_set_flags();

void test_get_8b_register_macros();
void test_set_8b_register_macros();

void test_get_next_byte();

void test_rotate_functions();




void test_get_low_byte(){
	struct registers reg;
	reg.af = 0xFF00; //1111111100000000
	reg.bc = 0xFF01; //1111111100000001
	reg.de = 0xFF02; //1111111100000010
	reg.hl = 0xFF03; //1111111100000011
	reg.sp = 0xFF04; //1111111100000100
	reg.pc = 0xFFFF; //1111111111111111
	unsigned char afLo = get_low_byte(reg.af);
	unsigned char bcLo = get_low_byte(reg.bc);
	unsigned char deLo = get_low_byte(reg.de);
	unsigned char hlLo = get_low_byte(reg.hl);
	unsigned char spLo = get_low_byte(reg.sp);
	unsigned char pcLo = get_low_byte(reg.pc);
	assert(afLo == 0);
	assert(bcLo == 1);
	assert(deLo == 2);
	assert(hlLo == 3);
	assert(spLo == 4);
	assert(pcLo == 0xFF);

    printf("get_low_byte tests passed\n");
}

void test_get_high_byte(){
	struct registers reg;
	reg.af = 0x00FF; //0000000011111111 top byte 0
	reg.bc = 0xAAFF; //1010101011111111 top byte 170
	reg.de = 0xFF00; //1111111100000000
	unsigned char afHi = get_high_byte(reg.af);
	unsigned char bcHi = get_high_byte(reg.bc);
	unsigned char deHi = get_high_byte(reg.de);

	assert(afHi == 0);
	assert(bcHi == 170);
	assert(deHi == 255);

    printf("get_high_byte tests passed\n");
}


void test_set_low_byte(){
	struct registers reg;
	reg.af = 0x0;
	//unsigned short *af = &reg.af;
	char val = 0x00FF;

	//printf("%i\n", reg.af);
	set_low_byte(&reg.af,val);
	assert(get_low_byte(reg.af) == 255);
	//printf("%i\n", reg.af);

	printf("set_low_byte tests passed\n");
}

void test_set_high_byte(){
	struct registers reg;
	reg.af = 0;
	//unsigned short *af = &reg.af;
	char val = 0xB7; // 10110111

	//printf("%i\n", reg.af);
	set_high_byte(&reg.af, val);
	unsigned char highByte = get_high_byte(reg.af);
	assert(highByte == 0xB7);
	//printf("%d\n", highByte);

	printf("set_high_byte tests passed\n");
}

void test_set_flags(){
    struct registers reg;
    struct registers *flagsPtr = &reg;

    reg.af = 0xF0; //11110000
    SET_ZF(flagsPtr, 0);
    assert(reg.af == 0x70);

    reg.af = 0xF0;
    SET_NF(flagsPtr, 0);
    assert(reg.af == 0xB0);

    reg.af = 0xF0;
    SET_HF(flagsPtr, 0);
    assert(reg.af == 0xD0);

    reg.af = 0xF0;
    SET_CF(flagsPtr, 0);
    assert(reg.af == 0xE0);


    reg.af = 0;
    SET_ZF(flagsPtr, 1);
    assert(reg.af == 0x80);

    reg.af = 0;
    SET_NF(flagsPtr, 1);
    assert(reg.af == 0x40);

    reg.af = 0;
    SET_HF(flagsPtr, 1);
    assert(reg.af == 0x20);

    reg.af = 0;
    SET_CF(flagsPtr, 1);
    assert(reg.af == 0x1);

    printf("set_flags tests passed\n");

}

void test_get_8b_registers(){
    struct registers reg;
    reg.af = 0xFF81; //1111111110000001 HB 255 LB 129
    reg.bc = 0xFF81;
    reg.de = 0xFF81;
    reg.hl = 0xFF81;

    assert(get_8b_register(&reg, 'a') == 255);
    assert(get_8b_register(&reg, 'f') == 129);
    assert(get_8b_register(&reg, 'b') == 255);
    assert(get_8b_register(&reg, 'c') == 129);
    assert(get_8b_register(&reg, 'd') == 255);
    assert(get_8b_register(&reg, 'e') == 129);
    assert(get_8b_register(&reg, 'h') == 255);
    assert(get_8b_register(&reg, 'l') == 129);

    printf("get_8b_registers tests passed\n");
}

void test_set_8b_registers(){
    struct registers reg;
    reg.af = 0;
    reg.bc = 0;
    reg.de = 0;
    reg.hl = 0;

    set_8b_register(&reg, 'a', 255);
    set_8b_register(&reg, 'c', 255);
    set_8b_register(&reg, 'd', 255);
    set_8b_register(&reg, 'e', 255);

    assert(reg.af == 65280);
    assert(reg.bc == 255);
    assert(reg.de == 65535);

    printf("set_8b_registers tests passed\n");
}

void test_get_8b_register_macros(){
    struct registers reg;
    init_registers(&reg);
    //printf("Register AF is %i\n", reg.af);
    reg.af = 1;
    reg.bc = 1;
    reg.de = 1;
    reg.hl = 1;

    //printf("Register AF is %i\n", reg.af);
    //char lowByte = get_low_byte((&reg)->af);
    struct registers *regPtrs = &reg;

    char lowByte = GET_F(regPtrs);
    char highByte = GET_A((&reg));
    assert(lowByte == 1);
    assert(highByte == 0);

    lowByte = GET_C((&reg));
    highByte = GET_B((&reg));
    assert(lowByte == 1);
    assert(highByte == 0);

    lowByte = GET_E((&reg));
    highByte = GET_D((&reg));
    assert(lowByte == 1);
    assert(highByte == 0);

    lowByte = GET_L((&reg));
    highByte = GET_H((&reg));
    assert(lowByte == 1);
    assert(highByte == 0);

    printf("get_8b_register_macros tests passed\n");
}

void test_set_8b_register_macros(){
    struct registers reg;
    init_registers(&reg);

    struct registers *regPtr = &reg;

    SET_F(regPtr, 1);
    SET_A(regPtr, 2);
    SET_B((&reg), 3);
    SET_C((&reg), 4);
    SET_D((&reg), 5);
    SET_E((&reg), 6);
    SET_H((&reg), 7);
    SET_L((&reg), 8);

    assert(get_low_byte(reg.af) == 1);
    assert(get_high_byte(reg.af) == 2);
    assert(get_high_byte(reg.bc) == 3);
    assert(get_low_byte(reg.bc) == 4);
    assert(get_high_byte(reg.de) == 5);
    assert(get_low_byte(reg.de) == 6);
    assert(get_high_byte(reg.hl) == 7);
    assert(get_low_byte(reg.hl) == 8);

    printf("set_8b_register_macros tests passed\n");
}

void test_get_next_byte(){
    struct registers reg;
    init_registers(&reg);

    //printf("pcounter: %i\n", reg.pc);

    char memory[0xFFFF] = {};

    memory[1] =  0x01; //LD B with something...

    //printf("1st Loc: %i\n2nd Loc:%i\n", memory[0], memory[1]);
    char byte1 = get_next_byte(&(reg.pc), memory);
    char byte2 = get_next_byte(&(reg.pc), memory);

    assert(byte1 == 0);
    assert(byte2 == 1);

    //printf("Byte1: %i\nByte2: %i\n", byte1, byte2);
    printf("test_get_next_byte tests passed\n");
}

void test_rotate_functions(){
    struct registers cpu;
    struct registers *cpuPtr = &cpu;
    init_registers(cpuPtr);
    //printf("%x\n", GET_F(cpuPtr));

    SET_A(cpuPtr, 0x83); //0x83 = 10000011
    SET_B(cpuPtr, 0x83);
    SET_C(cpuPtr, 0x83);
    SET_D(cpuPtr, 0x83);

    unsigned char newAVal = rot_right(GET_A(cpuPtr));
    SET_A(cpuPtr, newAVal);
    assert(GET_A(cpuPtr) == 0xC1); //0xC1 = 11000001
    SET_A(cpuPtr, 0);

    unsigned char newBVal = rot_right_carry(GET_B(cpuPtr), cpuPtr);
    SET_B(cpuPtr, newBVal);
    assert(GET_B(cpuPtr) == 0xC1); //0xC1 = 11000001
    //printf("F = %x\n", GET_F(cpuPtr));
    //printf("A = %x\n", GET_A(cpuPtr));
    assert(GET_F(cpuPtr) == 1);
    SET_F(cpuPtr, 0);

    unsigned char newCVal = rot_left(GET_C(cpuPtr));
    SET_C(cpuPtr, newCVal);
    //printf("C = %x\n", GET_C(cpuPtr));
    assert(GET_C(cpuPtr) == 0x7);
    SET_C(cpuPtr, 0);

    unsigned char newDVal = rot_left_carry(GET_D(cpuPtr), cpuPtr);
    SET_D(cpuPtr, newDVal);
    //printf("F = %x\n", GET_F(cpuPtr));
    //printf("D = %x\n", GET_D(cpuPtr));
    assert(GET_D(cpuPtr) == 7);
    assert(GET_F(cpuPtr) == 1);

    printf("rotate function tests passed\n");
}

void test_set_bit(){

    unsigned short cpuReg = 0;
    for (int i=0; i < 16; i++){
        set_bit(&cpuReg, i, 1);
        assert(cpuReg == (1 << i));
        cpuReg = 0;
    }

    printf("set bit tests passed\n");

}

void test_get_bit(){
    unsigned short cpuReg0 = 0;
    unsigned short cpuReg1 = 0xFFFF; //1111111111111111
    unsigned short bit0 = 0;
    unsigned short bit1 = 0;

    for (int i=0; i < 16; i++){
        bit0 = get_bit(&cpuReg0, i);
        bit1 = get_bit(&cpuReg1, i);
        assert(bit0 == 0);
        assert(bit1 == 1);
    }

    printf("get_bit tests passed\n");
}

void test_swap_nibble(){
    unsigned char toSwap = 0xF0;
    swap_nibble(&toSwap);
    assert(toSwap == 0x0F);

    printf("swap_nibble tests passed\n");
}

void test_compliment_carry_flag(){
    struct registers cpu;

    init_registers(&cpu);

    compliment_carry_flag(&cpu);
    assert(GET_CF((&cpu)) == 1);
    compliment_carry_flag(&cpu);
    assert(GET_CF((&cpu)) == 0);

    printf("compliment_carry_flag tests passed\n");

}

int main(){

	test_set_low_byte();
	test_set_high_byte();

	test_get_low_byte();
    test_get_high_byte();

    test_get_8b_registers();
    test_set_8b_registers();

    test_get_next_byte();
    test_get_8b_register_macros();
    test_set_8b_register_macros();

    //Bit operations
    test_set_bit();
    test_get_bit();
    test_rotate_functions();
    test_swap_nibble();
    test_compliment_carry_flag();
	return 0;
}

