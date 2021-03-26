#include <stdio.h>
#include <string.h>
#include "memory.h"

void init_mem(unsigned char *mem, int length){
    //Nice to work with clean memory
    memset(mem, 0, length * sizeof(*mem));
}

unsigned char read_mem(unsigned char *mem, unsigned short address){
    return mem[address];
}

int write_mem(unsigned char *mem, unsigned short address, unsigned char val){
    mem[address] = val;
    return 0;
}

