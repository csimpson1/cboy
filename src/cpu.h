#ifndef CPU_H_
#define CPU_H_

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

unsigned char get_bit(int val, int pos);
void set_bit_char(unsigned char *data, int pos, int value);

void swap_nibble(unsigned char *toSwap);
#endif // CPU_H_
