#include <assert.h>
#include <stdio.h>

#define __STR(x) #x
#define _RED(msg) "\033[31;1;4m" msg "\033[0m"
#define _GREEN(msg) "\033[32;1;4m" msg "\033[0m"

/* Test definitions */

#define testcase(name) static int name(void)

#define start_tests()                                                          \
    unsigned __t_passed = 0;                                                   \
    unsigned __t_run = 0;                                                      \
    do                                                                         \
        {                                                                      \
            printf(                                                            \
                "=========================================================\n"  \
                "RUNNING TESTS '"__FILE_NAME__                                 \
                "'\n"                                                          \
                "========================================================="    \
                "\n\n");                                                       \
        }                                                                      \
    while (0)

#define end_tests()                                                            \
    do                                                                         \
        {                                                                      \
            printf("\nPassed: "_GREEN("%u") "\tFailed: "_RED(                  \
                       "%u") "\tTotal: %u\n",                                  \
                   __t_passed, __t_run - __t_passed, __t_run);                 \
            if (__t_run - __t_passed != 0)                                     \
                {                                                              \
                    fprintf(stderr, "Result: "_RED("FAIL!") "\n");             \
                }                                                              \
            else                                                               \
                {                                                              \
                    printf("Result: "_GREEN("PASS!") "\n");                    \
                }                                                              \
        }                                                                      \
    while (0)

#define run_test(name)                                                         \
    do                                                                         \
        {                                                                      \
            int pass = name();                                                 \
            if (pass)                                                          \
                {                                                              \
                    printf(__STR(name) "::"_GREEN("PASS") "\n");               \
                    __t_passed++;                                              \
                }                                                              \
            else                                                               \
                {                                                              \
                    fprintf(stderr, __STR(name) "::"_RED("FAIL") "\n");        \
                }                                                              \
            __t_run++;                                                         \
        }                                                                      \
    while (0)

#define test_result_retcode() (__t_run - __t_passed == 0 ? 0 : -1);
