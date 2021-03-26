#include "bit_functions.h"

void _set_8b_to_16b(unsigned char *highByte, unsigned char *lowByte, unsigned short val){
    //Take a single 16b number, and store it as two 8b numbers in the  registers
    unsigned char highByteVal = ((unsigned char) (val >> 8) & 0xFF); //11111111
    unsigned char lowByteVal = ((unsigned char) val & 0xFF);
    *highByte = highByteVal;
    *lowByte = lowByteVal;

}

//get and set bits
unsigned char get_bit(int val, int pos){
    //To deal with the short and char types we use in the CPU struct,
    //cast them to an int first
    unsigned short mask = 1 << pos;
    unsigned char bitN = ((val) & mask) >> pos;
    return bitN;
}
