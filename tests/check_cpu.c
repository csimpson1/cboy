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

    //ck_assert_int_eq(get_bit( (int)testVal, 3), 0);
    //ck_assert_int_eq(get_bit( (int)testVal, 9), 1);

}
END_TEST


START_TEST(test_set_bit_char){
    CPU cpu = init_cpu();
    cpu.a = 0xF0; //11110000
    set_bit_char(&(cpu.a), 3, 1);
    ck_assert_int_eq(cpu.a, 0xF8);
}
END_TEST

START_TEST(test_swap_nibble){
    unsigned char toSwap = 0xFE;
    swap_nibble(&toSwap);
    ck_assert_int_eq(toSwap, 0xEF);
}
END_TEST


Suite * cpu_suite(){
    Suite* suite = suite_create("CPU");
    TCase *tcCore = tcase_create("Core");

    tcase_add_test(tcCore, test_init_cpu);
    tcase_add_test(tcCore, test_16b_byte_functions);
    tcase_add_test(tcCore, test_16b_register_functions);
    tcase_add_test(tcCore, test_get_bit_functions);
    //tcase_add_test(tcCore, test_set_bit_short);
    tcase_add_test(tcCore, test_set_bit_char);
    //tcase_add_test(tcCore, test_set_bit_16_register);
    tcase_add_test(tcCore, test_swap_nibble);
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
