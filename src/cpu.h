#ifndef CPU_H_
#define CPU_H_

#include "memory.h"
#include "bit_functions.h"
#include <stdio.h>
//The HC_CHECK for 8 bit numbers gets bits 0-3 from each number, and sees if the result would have caused a carry to bit 4
#define HC_CHECK(val1, val2) (((val1&0xf) + (val2&0xf))&0x10 == 0x10)
#define HC_CHECK_8B_SUB(val1, val2) (((val1 & 0xF) - (val2 & 0xF)) < 0)
#define HC_CHECK_16B_ADD(val1, val2) (((val1 & 0XFFF) + (val2 & 0xFFF)) & 0x1000 == 0x1000)
//We have an overflow if the result is smaller than either of the operands
#define C_CHECK(val1, val2, result) ( (result < val1) || (result < val2) )

#define SET_ZF(cpu, val) set_bit_char(&(cpu->f), 7, val)
#define SET_NF(cpu, val) set_bit_char(&(cpu->f), 6, val)
#define SET_CF(cpu, val) set_bit_char(&(cpu->f), 5, val)
#define SET_HF(cpu, val) set_bit_char(&(cpu->f), 4, val)

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

#define GET_AF(cpu) get_16b_register(cpu,1)
#define GET_BC(cpu) get_16b_register(cpu,2)
#define GET_DE(cpu) get_16b_register(cpu,3)
#define GET_HL(cpu) get_16b_register(cpu,4)
#define GET_PC(cpu) get_16b_register(cpu,5)
#define GET_SP(cpu) get_16b_register(cpu,6)

#define SET_AF(cpu, val) set_16b_register(cpu,1,val)
#define SET_BC(cpu, val) set_16b_register(cpu,2,val)
#define SET_DE(cpu, val) set_16b_register(cpu,3,val)
#define SET_HL(cpu, val) set_16b_register(cpu,4,val)
#define SET_PC(cpu, val) set_16b_register(cpu,5,val)
#define SET_SP(cpu, val) set_16b_register(cpu,6,val)



struct CPU {
    unsigned char a; // accumulator
    unsigned char f; // flags

    unsigned char b; // b-l are general purpose registers
    unsigned char c;

    unsigned char d;
    unsigned char e;

    unsigned char h;
    unsigned char l;

    unsigned short sp; // stack pointer
    unsigned short pc; // program counter

    int ime; // interrupt master flag enable
    unsigned char* memory[MEM_SIZE]; // memory

};

typedef struct CPU CPU;

void init_registers(CPU *cpu);

unsigned char get_high_byte(unsigned short *reg);
unsigned char get_low_byte(unsigned short *reg);
void set_high_byte(unsigned short *reg, unsigned char val);
void set_low_byte(unsigned short *reg, unsigned char value);

unsigned short get_16b_register(CPU *cpu, int reg);
void set_16b_register(CPU *cpu, int reg, unsigned short val);
unsigned short _get_8b_to_16b(unsigned char *highByte, unsigned char *lowByte);
void _set_8b_to_16b(unsigned char *highByte, unsigned char *lowByte, unsigned short val);

unsigned char swap_nibble(unsigned char *toSwap);
unsigned char sra(unsigned char reg, struct registers *cpu);
unsigned char srl(unsigned char reg, struct registers *cpu);
unsigned char sla(unsigned char reg, struct registers *cpu);

unsigned char rot_right(unsigned char reg, struct registers *cpu);
unsigned char rot_left(unsigned char reg, struct registers *cpu);
unsigned char rot_right_carry(unsigned char reg, struct registers *cpu);
unsigned char rot_left_carry(unsigned char reg, struct registers *cpu);

unsigned char get_byte(CPU *cpu, unsigned char *mem);
void increment_timer(unsigned char *mem, unsigned char cycles);
void flags_timer_post_inc(CPU *cpu, unsigned char *mem, unsigned char oldVal, unsigned char newVal);
void flags_timer_post_dec(CPU *cpu, char *mem, unsigned char oldVal, unsigned char newVal);

void interpret_opcode(CPU *cpu, unsigned char *mem, unsigned char opcode);
void interpret_extended_opcode(CPU *cpu, unsigned char *mem, unsigned char opcode);
#endif // CPU_H_
