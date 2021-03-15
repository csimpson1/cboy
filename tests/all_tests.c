#include "cutest.h"

CuSuite*  cpu_get_register_suite();

void run_all_tests(void){
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, cpu_get_register_suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

    int main(void) {
        run_all_tests();
    }
