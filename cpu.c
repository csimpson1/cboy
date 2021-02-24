/*
 * cpu.c
 *
 *  Created on: Jan. 20, 2021
 *      Author: csimp
 */

#include <stdio.h>
#include "cpu.h"


//High level register functions
void init_registers(struct registers *reg){
    //Initialize all registers to be zero
    reg->af = 0;
    reg->bc = 0;
    reg->de = 0;
    reg->hl = 0;
    reg->sp = 0;
    reg->pc = 0;
}

//Byte operations for 8b registers

unsigned char get_high_byte(unsigned short reg){
	unsigned char hb = reg >> 8;
	return hb;
}

unsigned char get_low_byte(unsigned short reg){
	unsigned char lb = reg & 0xFF; //11111111
	return lb;

}

int set_high_byte(unsigned short *reg, unsigned char val){
	unsigned short valToSet = (val << 8);
	//Mask the top 7 bytes of the reg
	*reg = *reg & 0xFF; // 0000000011111111
	*reg = *reg | valToSet;
	return 0;
}

int set_low_byte(unsigned short *reg, unsigned char value){
	*reg = *reg & 0xFF00; // 1111111100000000
	*reg = *reg | value;
	return 0;
}

//TODO: Rewrite this so it can set any position in a 16 bit short. We'll need to set bits on many other registers
//
//
int set_bit(unsigned short *reg, int pos ,unsigned char value){
    //We're setting an individual bit, so reject any value that doesn't make sense

    if (value != 0 && value !=1){
        return 1;
    }

    //flip the bits in the mask
    unsigned short mask = 1 << pos;
    mask = ~mask;

    short valToSet = (value << pos);
    //Zero out the position we want

    *reg = *reg & mask;
    *reg = *reg | valToSet;

    return 0;
}

unsigned short get_bit(unsigned short *reg, int pos ){
    unsigned short mask = 1 << pos;
    unsigned short bitN = ((*reg) & mask) >> pos;
    return bitN;

}

unsigned char swap_nibble(unsigned char reg){
    unsigned char lower = reg & 0xF;  // 00001111
    unsigned char upper = reg & 0xF0; // 11110000

    lower = lower << 4;
    upper = upper >> 4;

    unsigned char result = upper | lower;
    return result;
}

unsigned char sra(unsigned char reg, struct registers *cpu){
    //Shift right the contents of a register, set bit 7 to 0. Contents of 0 bit are copied to c flag
    unsigned char bit0 = reg & 1;
    reg = ((reg >> 1) & 0x7F); //01111111
    SET_CF(cpu, bit0);
    return reg;
}

unsigned char srl(unsigned char reg, struct registers *cpu){
    //Shift right the contents of a register, leave bit 7 be. Contents of 0 bit are copied to c flag
    unsigned char bit7 = reg & 0x80; //10000000
    unsigned char bit0 = reg & 1;

    reg = reg >> 1;
    reg = (reg & 0x7f) | bit7;
    SET_CF(cpu, bit0);
    return reg;
}

unsigned char sla(unsigned char reg, struct registers *cpu){
    //Shift left the contents of a register, set bit 0 to 0. Contents of bit 7 are copied to c flag
    unsigned char bit7 = (reg & 0x80) >> 7; //10000000
    reg = reg << 1;
    SET_CF(cpu, bit7);\
    return reg;
}

void compliment_carry_flag(struct registers *cpu){
    unsigned char cf = GET_CF(cpu);

    switch(cf){
        case 0:{
            SET_CF(cpu, 1);
            break;
        }

        case 1:{
            SET_CF(cpu, 0);
            break;
        }

    }

    return;
}

//arithmetic operations

unsigned char add(unsigned char a, unsigned char reg, struct registers *cpu){
    unsigned char sum = a + reg;

    //flags
    check_and_set_zf(sum, cpu);
    SET_NF(cpu, 0);
    check_and_set_cf_8b(a, reg, sum, cpu,0);

    return sum;
}

unsigned char adc(unsigned char a, unsigned char reg, struct registers *cpu){
    unsigned char sum = a + reg + GET_CF(cpu);

    //flags
    check_and_set_zf(sum, cpu);
    SET_NF(cpu, 0);
    check_and_set_cf_8b(a, reg, sum, cpu,0);

    return sum;
}

unsigned char sub(unsigned char a, unsigned char reg, struct registers *cpu){
    unsigned char dif = a - reg;
    return dif;
}

unsigned char sbc(unsigned char a, unsigned char reg, struct registers *cpu){
    unsigned char dif = a - reg - GET_CF(cpu);
    return dif;
}

//conditional flag setting operations
int check_and_set_zf(unsigned char result, struct registers *cpu){
    unsigned char toSet;
    if(result == 0){
        toSet = 1;

    }
    else{
        SET_CF(cpu, 0);
        toSet = 0;
    }

    SET_CF(cpu, toSet);
    return 1;
}


int check_and_set_hf_8b(unsigned char arg1, unsigned char arg2, struct registers *cpu){
    /*
    Get the low 4 bytes of each operand and add them together. Then see if we have a bit flipped at position 4
    */
    unsigned char toSet;

    if((((0xF & arg1) + (0xF & arg2))&0x10) == 0x10){
        toSet = 1;
    }

    else{
        toSet = 0;
    }

    SET_HF(cpu, toSet);
    return (int)toSet;
}

int check_and_set_hf_16b(unsigned char arg1, unsigned char arg2, struct registers *cpu){
    /*
    Get bits 8-11 of the arguments, and add them together. Then see if there is a flipped bit at position 12
    */
    unsigned short toSet;
    if((((0xF00 & arg1) + (0xF00 & arg2)) & 0x1000) == 0x1000){
        toSet = 1;
    }
    else{
        toSet = 0;
    }

    SET_HF(cpu, toSet);
    return (int)toSet;

}
int check_and_set_cf_8b(unsigned char arg1, unsigned char arg2, unsigned char result, struct registers *cpu, unsigned char mode){
    /*
     Mode specifies if we are dealing with 8 or 16 bit integers, and an addition or subtraction
     Mode 0: 8 bit add
     Mode 1: 8 bit sub


     When adding we have a carry operation if either

     1) Our result is bigger than 8 (16) bits
     2) Our result is smaller than either of the arguments (we overflowed)

     When subtracting unsigned numbers we will see an overflow
     1) Our result will be larger than either of the arguments (?)
     */

     switch(mode){
        unsigned char toSet;
        case 0:{
            if(result < arg1 || result < arg2){
                toSet = 1;
            }

            else{
                toSet = 0;
            }

            SET_CF(cpu, toSet);
            return (int)toSet;

        }

        case 1:{
            if (result > arg1 || result > arg2){
                toSet = 1;

            }

            else{
                toSet = 0;
            }

            SET_CF(cpu, toSet);
            return (int)toSet;
        }

        default:{
            return -1;
        }

     }
}

int check_and_set_cf_16b(unsigned short arg1, unsigned short arg2, unsigned short result, struct registers *cpu, unsigned char mode){
    /*
    This is annoying. Given that I can't overload the function, how can we reduce the amt of code?
     Mode specifies if we are dealing with 8 or 16 bit integers, and an addition or subtraction
     Mode 0: 8 bit add
     Mode 1: 8 bit sub


     When adding we have a carry operation if either

     1) Our result is bigger than 8 (16) bits
     2) Our result is smaller than either of the arguments (we overflowed)

     When subtracting unsigned numers we will see an overflow
     1) Our result will be larger than either of the arguments (?)
     */

     switch(mode){
        unsigned char toSet;
        case 0:{
            //if(result > 0xFFFF || result < arg1 || result < arg2){
            if(result < arg1 || result < arg2){
                toSet = 1;
            }

            else{
                toSet = 0;
            }

            SET_CF(cpu, toSet);
            return (int)toSet;

        }

        case 1:{
            if (result > arg1 || result > arg2){
                toSet = 1;

            }

            else{
                toSet = 0;
            }

            SET_CF(cpu, toSet);
            return (int)toSet;
        }

        default:{
            return -1;
        }

     }

}



unsigned char get_8b_register(struct registers *reg, char name){
	char regValue;

	switch(name){
		case 'a':
			regValue = get_high_byte(reg->af);
			break;

		case 'f':
			regValue = get_low_byte(reg->af);
			break;

		case 'b':
			regValue = get_high_byte(reg -> bc);
			break;

		case 'c':
			regValue = get_low_byte(reg -> bc);
			break;

		case 'd':
			regValue = get_high_byte(reg -> de);
			break;

		case 'e':
			regValue = get_low_byte(reg -> de);
			break;

		case 'h':
			regValue = get_high_byte(reg -> hl);
			break;

		case 'l':
			regValue = get_low_byte(reg -> hl);
			break;

		default:
			//Is there a better way to do this? This should indicate an error
			regValue = 0;
			break;
	}


	return regValue;
}

int set_8b_register(struct registers *reg, char name, unsigned char val){
	int retVal = 1;

	switch(name){
		case 'a':
			set_high_byte(&(reg->af), val);
			break;

		case 'f':
			set_low_byte(&(reg->af), val);
			break;

		case 'b':
			set_high_byte(&(reg -> bc), val);
			break;

		case 'c':
			set_low_byte(&(reg -> bc), val);
			break;

		case 'd':
			set_high_byte(&(reg -> de), val);
			break;

		case 'e':
			set_low_byte(&(reg -> de), val);
			break;

		case 'h':
			set_high_byte(&(reg -> hl), val);
			break;

		case 'l':
			set_low_byte(&(reg -> hl), val);
			break;

		default:
			//Is there a better way to do this? This should indicate an error
			retVal = 0;
			break;
	}


	return retVal;
}

//Rotate functions
unsigned char rot_right(unsigned char reg){
    unsigned char bit0 = reg & 0x1;
    unsigned char rotated = reg >> 1;
    rotated = (rotated & 0x7F) | (bit0 << 7); // 0x7F=01111111

    return rotated;
}

unsigned char rot_left(unsigned char reg){
    unsigned char bit7 = (reg & 0x80) >> 7; // 0x80=10000000
    unsigned char rotated = reg << 1;
    rotated = (rotated & 0xFE) | bit7; // 0xFE=11111110

    return rotated;
}

unsigned char rot_right_carry(unsigned char reg, struct registers *cpu){
    unsigned char bit0 = reg & 0x1;
    unsigned char rotated = reg >> 1;
    rotated = (rotated & 0x7F) | (bit0 << 7); // 0x7F=01111111
    SET_CF(cpu, bit0);

    return rotated;
}

unsigned char rot_left_carry(unsigned char reg, struct registers *cpu){
    unsigned char bit7 = (reg & 0x80) >> 7; // 0x80=10000000
    unsigned char rotated = reg << 1;
    rotated = (rotated & 0xFE) | bit7; // 0xFE=11111110

    SET_CF(cpu, bit7);

    return rotated;
}



unsigned char get_next_byte(unsigned short *pCounter, char *mem){
    //return the value in memory specified by the program counter, and increment it to point to the next byte
    int pCounterInt = *pCounter;
    //printf("pCounter: %i\n", *pCounter);
    *pCounter += 1;
    //printf("pCounter: %i\n", *pCounter);
    return mem[pCounterInt];
}

int interpret_opcode(struct registers *reg, char opcode, char *mem, int *cycleCounter){


    switch(opcode){
        case 0x00:{ //NOP
            //Increment PC and do nothing else
            *cycleCounter += 1; ;
            break;
        }

        case 0x01:{ //LD BC, d16
            /*
             * Load the next 2 bytes into register BC
             * The first byte is stored into C, the second byte into B
             */
             unsigned char lByte;
             unsigned char hByte;
             lByte = get_next_byte(&(reg->pc), mem);
             hByte = get_next_byte(&(reg->pc), mem);

             set_low_byte(&(reg->bc), lByte);
             set_high_byte(&(reg->bc), hByte);
             *cycleCounter += 3;
             break;
        }

        case 0x02:{ //LD (BC), A
            /*
             * Take the value in the accumulator and store it in the memory
             * address specified by (BC)
             */
             ;
             unsigned char accVal = get_high_byte(reg->af);
             int address = reg->bc;
             mem[address] = accVal;
             *cycleCounter += 2;
             break;
        }

        case 0x03:{ //INC BC
            //Add 1 to BC

            reg->bc += 1;
            *cycleCounter += 1;
            break;
        }

        case 0x04:{ //INC B
            //Add 1 to B
            unsigned char b = GET_B(reg);
            b++;
            SET_B(reg, b);

            *cycleCounter += 1;
            break;

        }

        case 0x05:{

        }

    }

    return 0;
}
