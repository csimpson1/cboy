/*
 * cpu.h
 *
 *  Created on: Jan. 22, 2021
 *      Author: csimp
 */

#ifndef SRC_CPU_H_
#define SRC_CPU_H_



#endif /* SRC_CPU_H_ */


struct registers {
	unsigned short af; //accumulator & flags
	unsigned short bc;
	unsigned short de;
	unsigned short hl;
	unsigned short sp; //stack pointer
	unsigned short pc; //program counter
};

char memory[0xFFFF];
//High level memory functions
void init_registers(struct registers *reg);
//void init_memory(char *mem)

//Byte functions for the 8b registers
unsigned char get_high_byte(unsigned short reg);
unsigned char get_low_byte(unsigned short reg);
int set_high_byte(unsigned short *reg, unsigned char val);
int set_low_byte(unsigned short *reg, unsigned char val);


//8b helper functions
unsigned char get_8b_register(struct registers *reg, char name);
int set_8b_register(struct registers *reg,  char name, unsigned char val);

//16b helper functions
short get_16b_register(struct registers *reg, char name);
int set_16b_register(struct registers *reg, char name, unsigned short val);

//Fetch instructions
unsigned char get_next_byte(unsigned short *pcounter, char *mem);

//interpret instructions
int interpret_opcode(struct registers *reg, char opcode, char *mem, int *cycleCounter);

//rotate instructions
unsigned char rot_right(unsigned char reg);
unsigned char rot_left(unsigned char reg);
unsigned char rot_right_carry(unsigned char reg, struct registers *cpu);
unsigned char rot_left_carry(unsigned char reg, struct registers *cpu);

//bitwise instructions
unsigned short get_bit(unsigned short *reg, int pos );
int set_bit(unsigned short *reg, int pos ,unsigned char value);
unsigned short get_bit(unsigned short *reg, int pos);
unsigned char swap_nibble(unsigned char reg);
unsigned char sra(unsigned char reg, struct registers *cpu);
unsigned char srl(unsigned char reg, struct registers *cpu);
unsigned char sla(unsigned char reg, struct registers *cpu);
void compliment_carry_flag(struct registers *cpu);

//arithmetic instructions
unsigned char add(unsigned char a, unsigned char reg, struct registers *cpu);
unsigned char adc(unsigned char a, unsigned char reg, struct registers *cpu);
unsigned char sub(unsigned char a, unsigned char reg, struct registers *cpu);
unsigned char sbc(unsigned char a, unsigned char reg, struct registers *cpu);

//conditional flag setting operations
int check_and_set_zf(unsigned char result, struct registers *cpu);
//No check_and_set_nf is needed, as we only set the flag if we did a subtraction
int check_and_set_hf(unsigned char arg1, unsigned char arg2, unsigned char result, struct registers *cpu);
int check_and_set_cf_8b(unsigned char arg1, unsigned char arg2, unsigned char result, struct registers *cpu, unsigned char mode);
//Helpful Macros

#define GET_A(rPtr) get_high_byte(rPtr->af)
#define GET_F(rPtr) get_low_byte((rPtr->af))
#define GET_B(rPtr) get_high_byte(rPtr->bc)
#define GET_C(rPtr) get_low_byte(rPtr->bc)
#define GET_D(rPtr) get_high_byte(rPtr->de)
#define GET_E(rPtr) get_low_byte(rPtr->de)
#define GET_H(rPtr) get_high_byte(rPtr->hl)
#define GET_L(rPtr) get_low_byte(rPtr->hl)


#define SET_A(r, val) set_high_byte(&(r->af), val)
#define SET_F(r, val) set_low_byte(&(r->af), val)
#define SET_B(r, val) set_high_byte(&(r->bc), val)
#define SET_C(r, val) set_low_byte(&(r->bc), val)
#define SET_D(r, val) set_high_byte(&(r->de), val)
#define SET_E(r, val) set_low_byte(&(r->de), val)
#define SET_H(r, val) set_high_byte(&(r->hl), val)
#define SET_L(r, val) set_low_byte(&(r->hl), val)

#define SET_ZF(r, val) set_bit(&(r->af), 3, val)
#define SET_NF(r, val) set_bit(&(r->af), 2, val)
#define SET_HF(r, val) set_bit(&(r->af), 1, val)
#define SET_CF(r, val) set_bit(&(r->af), 0, val)

#define GET_ZF(r) get_bit(&(r->af), 3)
#define GET_NF(r) get_bit(&(r->af), 2)
#define GET_HF(r) get_bit(&(r->af), 1)
#define GET_CF(r) get_bit(&(r->af), 0)

