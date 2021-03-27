#include <stdio.h>
#include <string.h>
#include "memory.h"

void init_mem(unsigned char *mem, int length){
    //Nice to work with clean memory
    memset(mem, 0, length * sizeof(*mem));
}

unsigned char read_mem(unsigned char *mem, unsigned short address){
    //implement sprite OAM bug?
    switch(address){
        // Channel 1 Frequency Low Register is not readable
        case NR13:{
            return 0x0;
        }

        //Channel 2 Frequency Low Register is not readable
        case NR23:{
            return 0x0;
        }

        //Channel 3 Frequency Low Register is not readable
        case NR33:{
            return 0x0;
        }

        default:{
            return mem[address];
        }
    }

}

int write_mem(unsigned char *mem, unsigned short address, unsigned char val){
    mem[address] = val;

    //Handle writes to areas in scope of echo ram
    if(WRAM_0_L <= address && address <= 0xDDFF){
        mem[address + 0x2000] = val;
    }

    else if(ECHO_RAM_L <= address && address <= ECHO_RAM_L){
        mem[address - 0x2000] = val;
    }

    return 0;
}

