#include "cpu.h"
#include "bit_functions.h"
#include <stdio.h>

void init_registers(CPU *cpu){
    cpu->a = 0;
    cpu->f = 0;

    cpu->b = 0;
    cpu->c = 0;

    cpu->d = 0;
    cpu->e = 0;

    cpu->h = 0;
    cpu->l = 0;

    cpu->pc = 0;
    cpu->sp = 0;
}

//Byte operations for 16b registers

unsigned char get_high_byte(unsigned short *reg){
	unsigned char hb = *reg >> 8;
	return hb;
}

unsigned char get_low_byte(unsigned short *reg){
	unsigned char lb = *reg & 0xFF; //11111111
	return lb;

}

void set_high_byte(unsigned short *reg, unsigned char val){
	unsigned short valToSet = (val << 8);
	//Mask the top 7 bytes of the reg
	*reg = *reg & 0xFF; // 0000000011111111
	*reg = *reg | valToSet;
}

void set_low_byte(unsigned short *reg, unsigned char value){
	*reg = *reg & 0xFF00; // 1111111100000000
	*reg = *reg | value;
}

//Get and set operations for 16b registers
/*
    Will use the following shorthand for registers
    1: AF
    2: BC
    3: DE
    4: HL
    5: PC
    6: SP
*/
unsigned short get_16b_register(CPU *cpu, int reg){
    unsigned short regValue;

    switch(reg){
        case 1:{
            regValue = _get_8b_to_16b(&(cpu->a), &(cpu->f));
            break;
        }
        case 2:{
            regValue = _get_8b_to_16b(&(cpu->b), &(cpu->c));
            break;
        }
        case 3:{
            regValue = _get_8b_to_16b(&(cpu->d), &(cpu->e));
            break;
        }
        case 4:{
            regValue = _get_8b_to_16b(&(cpu->h), &(cpu->l));
            break;
        }
        case 5:{
            regValue = cpu->pc;
            break;
        }
        case 6:{
            regValue = cpu->sp;
            break;
        }

    }
    return regValue;
}

void set_16b_register(CPU *cpu, int reg, unsigned short val){
    unsigned short regValue;
    switch(reg){
        case 1:{
            _set_8b_to_16b(&(cpu->a), &(cpu->f), val);
            break;
        }
        case 2:{
            _set_8b_to_16b(&(cpu->b), &(cpu->c), val);
            break;
        }
        case 3:{
            _set_8b_to_16b(&(cpu->d), &(cpu->e), val);
            break;
        }
        case 4:{
            _set_8b_to_16b(&(cpu->h), &(cpu->l), val);
            break;
        }
        case 5:{
            cpu->pc = val;
            break;
        }
        case 6:{
            cpu->sp = val;
            break;
        }
    }

}

unsigned short _get_8b_to_16b(unsigned char *highByte, unsigned char *lowByte){
    //Take two 8b numbers from the registers, and interpret them as a single 16b number
    unsigned short highByteN = (unsigned short) (*highByte);
    unsigned short lowByteN = (unsigned short) (*lowByte);

    highByteN = (highByteN << 8) & 0xFF00; //1111111100000000
    lowByteN = lowByteN & 0xFF; // 11111111

    return (highByteN | lowByteN);
}



void set_bit_char(unsigned char *data, int pos, int value){
     //We're setting an individual bit, so reject any value that doesn't make sense

    if (value != 0 && value !=1){
        return;
    }

    //flip the bits in the mask
    unsigned short mask = 1 << pos;
    mask = ~mask;

    short valToSet = (value << pos);
    //Zero out the position we want

    *data = *data & mask;
    *data = *data | valToSet;


}

//swap nibble
void swap_nibble(unsigned char *toSwap){
    unsigned char highNib = (*toSwap & 0xF0) >> 4;
    unsigned char lowNib = (*toSwap & 0x0F) <<4;

    *toSwap = lowNib | highNib;
}



