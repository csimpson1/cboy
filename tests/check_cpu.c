#include "cutest.h"
#include "../cpu.h"
/*
START_TEST(test_swap_nibble){
    unsigned char toSwap = 0xF0;
    unsigned char swapped = swap_nibble(toSwap);
    ck_assert_int_eq(swapped, 0x0F);
}
END_TEST

int main(void){
    return 0;
}
*/

/*
Helper functions
*/

struct registers set_up_cpu(){
    //Basic setup for a test
    struct registers cpu;
    init_registers(&cpu);
    return cpu;
}

/*
Tests for the bitwise instructions
unsigned short get_bit(unsigned short *reg, int pos );
int set_bit(unsigned short *reg, int pos ,unsigned char value);
unsigned char swap_nibble(unsigned char reg);
unsigned char sra(unsigned char reg, struct registers *cpu);
unsigned char srl(unsigned char reg, struct registers *cpu);
unsigned char sla(unsigned char reg, struct registers *cpu);
void compliment_carry_flag(struct registers *cpu);
*/

void test_get_bit(CuTest *tc){
    unsigned short cpuReg0 = 0;
    unsigned short cpuReg1 = 0xFFFF; //1111111111111111
    unsigned short bit0 = 0;
    unsigned short bit1 = 0;

    for (int i=0; i < 16; i++){
        bit0 = get_bit(&cpuReg0, i);
        bit1 = get_bit(&cpuReg1, i);
        CuAssertTrue(tc, bit0 == 0);
        CuAssertTrue(tc, bit1 == 1);
    }
}

void test_set_bit(CuTest *tc){
    unsigned short cpuReg = 0;
    for (int i=0; i < 16; i++){
        set_bit(&cpuReg, i, 1);
        CuAssertTrue(tc, cpuReg == (1 << i));
        cpuReg = 0;
    }
}

void test_swap_nibble(CuTest *tc){
    unsigned char toSwap = 0xF0;
    unsigned char swapped = swap_nibble(toSwap);
    CuAssertTrue(tc, swapped == 0x0F);
}

void test_sra(CuTest *tc){
    //Create a new pointer to a registers struct
    struct registers cpu = set_up_cpu();

    SET_A((&cpu), 0xFF); //11111111
    unsigned char newAVal = sra(GET_A((&cpu)), &cpu);
    SET_A((&cpu), newAVal);

    CuAssertTrue(tc, GET_A((&cpu)) == 0x7F); //01111111
    CuAssertTrue(tc, GET_CF((&cpu)) == 1);
}

void test_srl(CuTest *tc){

    struct registers cpu = set_up_cpu();

    SET_B((&cpu), 0xBE); //10111110
    unsigned char newBVal = srl(GET_B((&cpu)), &cpu);
    SET_B((&cpu), newBVal);
    CuAssertTrue(tc, GET_B((&cpu)) == 0xDF); //11011111
    CuAssertTrue(tc, GET_CF((&cpu)) == 0);
}

void test_sla(CuTest *tc){
    struct registers cpu = set_up_cpu();

    SET_C((&cpu), 0xDF);
    unsigned char newCVal = sla(GET_C((&cpu)), &cpu);
    SET_C((&cpu), newCVal);
    CuAssertTrue(tc, GET_C((&cpu)) == 0xBE);
    CuAssertTrue(tc, GET_CF((&cpu)) == 1);

}

void test_compliment_carry_flag(CuTest *tc){
    struct registers cpu = set_up_cpu();

    compliment_carry_flag(&cpu);
    CuAssertTrue(tc, GET_CF((&cpu)) == 1);
    compliment_carry_flag(&cpu);
    CuAssertTrue(tc, GET_CF((&cpu)) == 0);
}

CuSuite* cpu_get_register_suite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_get_bit);
    SUITE_ADD_TEST(suite, test_set_bit);
    SUITE_ADD_TEST(suite, test_swap_nibble);
    SUITE_ADD_TEST(suite, test_sra);
    SUITE_ADD_TEST(suite, test_srl);
    SUITE_ADD_TEST(suite, test_sla);
    SUITE_ADD_TEST(suite, test_compliment_carry_flag);

    return suite;
}
