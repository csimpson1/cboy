#include "check.h"
#include "../src/cpu.h"
#include <stdlib.h>
#include <stdio.h>
/* Helper Functions */
CPU init_cpu(){
    CPU cpu;
    init_registers(&cpu);

    return cpu;
}

START_TEST(test_init_cpu){
    CPU cpu = init_cpu();

    ck_assert_int_eq(cpu.a, 0);
    ck_assert_int_eq(cpu.f, 0);
    ck_assert_int_eq(cpu.b, 0);
    ck_assert_int_eq(cpu.c, 0);
    ck_assert_int_eq(cpu.d, 0);
    ck_assert_int_eq(cpu.e, 0);
    ck_assert_int_eq(cpu.h, 0);
    ck_assert_int_eq(cpu.l, 0);
    ck_assert_int_eq(cpu.pc, 0);
    ck_assert_int_eq(cpu.sp, 0);
}
END_TEST

START_TEST(test_16b_byte_functions){
    CPU cpu = init_cpu();

    cpu.sp = 0xFFEE;
    //printf("%i", cpu.sp)
    ck_assert_int_eq(0xFF, get_high_byte(&(cpu.sp)));
    ck_assert_int_eq(0xEE, get_low_byte(&(cpu.sp)));

    set_high_byte(&(cpu.sp), 0xAA);
    set_low_byte(&(cpu.sp), 0xBB);

    ck_assert_int_eq(0xAA, get_high_byte(&(cpu.sp)));
    ck_assert_int_eq(0xBB, get_low_byte(&(cpu.sp)));
}
END_TEST

START_TEST(test_16b_register_functions){
    CPU cpu = init_cpu();

    cpu.a = 0xFF; cpu.f = 0x1;
    cpu.b = 0xFF; cpu.c = 0x2;
    cpu.d = 0xFF; cpu.e = 0x3;
    cpu.h = 0xFF; cpu.l = 0x4;
    cpu.sp = 0xFFFF;
    cpu.pc = 0xFFFE;


    ck_assert_int_eq(get_16b_register(&cpu, 1), 0xFF01);
    ck_assert_int_eq(get_16b_register(&cpu, 2), 0xFF02);
    ck_assert_int_eq(get_16b_register(&cpu, 3), 0xFF03);
    ck_assert_int_eq(get_16b_register(&cpu, 4), 0xFF04);
    ck_assert_int_eq(get_16b_register(&cpu, 5), 0xFFFE);
    ck_assert_int_eq(get_16b_register(&cpu, 6), 0xFFFF);

    set_16b_register(&cpu, 1, 0x1FF);
    set_16b_register(&cpu, 2, 0x2FF);
    set_16b_register(&cpu, 3, 0x3FF);
    set_16b_register(&cpu, 4, 0x4FF);
    set_16b_register(&cpu, 5, 0x5FF);
    set_16b_register(&cpu, 6, 0x6FF);

    ck_assert_int_eq(get_16b_register(&cpu, 1), 0x1FF);
    ck_assert_int_eq(get_16b_register(&cpu, 2), 0x2FF);
    ck_assert_int_eq(get_16b_register(&cpu, 3), 0x3FF);
    ck_assert_int_eq(get_16b_register(&cpu, 4), 0x4FF);
    ck_assert_int_eq(get_16b_register(&cpu, 5), 0x5FF);
    ck_assert_int_eq(get_16b_register(&cpu, 6), 0x6FF);

}
END_TEST

START_TEST(test_get_bit_functions){
    CPU cpu = init_cpu();
    cpu.a = 0xF0; //11110000
    unsigned short testVal = 0xFF00; // 1111111100000000

    ck_assert_int_eq(get_bit( (int)(cpu.a), 3), 0);
    ck_assert_int_eq(get_bit( (int)(cpu.a), 4), 1);

    ck_assert_int_eq(get_bit( (int)testVal, 3), 0);
    ck_assert_int_eq(get_bit( (int)testVal, 9), 1);

}
END_TEST

START_TEST(test_set_bit_short){
    unsigned short testVal = 0xFF00; // 1111111100000000
    set_bit_short(&testVal, 7, 1);
    ck_assert_int_eq(testVal, 0xFF80);
}
END_TEST

START_TEST(test_set_bit_char){
    CPU cpu = init_cpu();
    cpu.a = 0xF0; //11110000
    set_bit_char(&(cpu.a), 3, 1);
    ck_assert_int_eq(cpu.a, 0xF8);
}
END_TEST

START_TEST(test_set_bit_16_register){
    CPU cpu = init_cpu();
    //Case 1: Set a native 16b register
    set_bit_16b_reg(&cpu, 6, 0, 1);
    set_bit_16b_reg(&cpu, 6, 1, 1);

    ck_assert_int_eq(cpu.sp, 0x3);

    //Case 2: Choose a position which will cause a high byte to be set
    set_bit_16b_reg(&cpu, 2, 0, 1);
    set_bit_16b_reg(&cpu, 2, 1, 1);

    ck_assert_int_eq(cpu.c, 0x3);

    //Case 3: Choose a position which will cause a low byte to be set
    set_bit_16b_reg(&cpu, 2, 8, 1);
    set_bit_16b_reg(&cpu, 2, 9, 1);

    ck_assert_int_eq(cpu.b, 0x3);


}
END_TEST
/*
Tests for the bitwise instructions
unsigned short get_bit(unsigned short *reg, int pos );
int set_bit(unsigned short *reg, int pos ,unsigned char value);
unsigned char swap_nibble(unsigned char reg);
unsigned char sra(unsigned char reg, struct registers *cpu);
unsigned char srl(unsigned char reg, struct registers *cpu);
unsigned char sla(unsigned char reg, struct registers *cpu);
void compliment_carry_flag(struct registers *cpu);


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
*/

Suite * cpu_suite(){
    Suite* suite = suite_create("CPU");
    TCase *tcCore = tcase_create("Core");

    tcase_add_test(tcCore, test_init_cpu);
    tcase_add_test(tcCore, test_16b_byte_functions);
    tcase_add_test(tcCore, test_16b_register_functions);
    tcase_add_test(tcCore, test_get_bit_functions);
    tcase_add_test(tcCore, test_set_bit_short);
    tcase_add_test(tcCore, test_set_bit_char);
    tcase_add_test(tcCore, test_set_bit_16_register);
    suite_add_tcase(suite, tcCore);
    /*
    SUITE_ADD_TEST(suite, test_get_bit);
    SUITE_ADD_TEST(suite, test_set_bit);
    SUITE_ADD_TEST(suite, test_swap_nibble);
    SUITE_ADD_TEST(suite, test_sra);
    SUITE_ADD_TEST(suite, test_srl);
    SUITE_ADD_TEST(suite, test_sla);
    SUITE_ADD_TEST(suite, test_compliment_carry_flag);
    */

    return suite;
}

int main(){
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = cpu_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
