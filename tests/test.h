#include <assert.h>
#include <stdio.h>

#define __STR(x) #x

/* Test definitions */

#define testcase(name) static int name(void)

#define start_tests()                                                          \
    unsigned __t_passed = 0;                                                   \
    unsigned __t_run = 0;                                                      \
    do                                                                         \
        {                                                                      \
            printf("RUNNING %s TESTS\n\n", __FILE_NAME__);                     \
        }                                                                      \
    while (0)

#define end_tests()                                                            \
    do                                                                         \
        {                                                                      \
            printf(                                                            \
                "\n=====Summary=====\nPassed: %u\nFailed: %u\nTotal: %u\n\n",  \
                __t_passed, __t_run - __t_passed, __t_run);                    \
            if (__t_run - __t_passed != 0)                                     \
                {                                                              \
                    fprintf(stderr, "Result: FAIL!\n");                        \
                }                                                              \
            else                                                               \
                {                                                              \
                    printf("Result: PASS!\n");                                 \
                }                                                              \
        }                                                                      \
    while (0)

#define run_test(name)                                                         \
    do                                                                         \
        {                                                                      \
            int pass = name();                                                 \
            if (pass)                                                          \
                {                                                              \
                    printf("%s::PASS\n", __STR(name));                         \
                    __t_passed++;                                              \
                }                                                              \
            else                                                               \
                {                                                              \
                    fprintf(stderr, "%s::FAIL\n", __STR(name));                \
                }                                                              \
            __t_run++;                                                         \
        }                                                                      \
    while (0)

#define test_result_retcode() (__t_run - __t_passed == 0 ? 0 : -1);
