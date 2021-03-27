#include "bit_functions.h"

//get and set bits
unsigned char get_bit(int val, int pos){
    //To deal with the short and char types we use in the CPU struct,
    //cast them to an int first
    unsigned short mask = 1 << pos;
    unsigned char bitN = ((val) & mask) >> pos;
    return bitN;
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
