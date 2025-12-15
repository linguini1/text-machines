#include <stdio.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

testcase(test_invalid_file)
{
    struct txtmac *filemac = minit_file(NULL);
    assert(filemac == NULL);
    return 1;
}

testcase(test_helloworld_file)
{
    FILE *file = fopen("examples/hello.txt", "r");
    assert(file != NULL);

    struct txtmac *fmac = minit_file(file);
    assert(fmac != NULL);

    assert(fmac->next(fmac) == 'H');
    assert(fmac->next(fmac) == 'e');
    assert(fmac->next(fmac) == 'l');

    mdestroy(fmac);
    fclose(file);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_invalid_file);
    run_test(test_helloworld_file);

    end_tests();
    return test_result_retcode();
}
