#include "../machines/machines.h"
#include "test.h"

testcase(test_one_letter)
{
    struct txtmac *tm = minit_letter('a', 1);
    assert(tm != NULL);

    assert(tm->next(tm) == 'a');
    assert(tm->next(tm) == EOF);
    return 1;
}

testcase(test_five_letters)
{
    struct txtmac *tm = minit_letter('m', 5);
    assert(tm != NULL);

    for (int i = 0; i < 5; i++)
        {
            assert(tm->next(tm) == 'm');
        }

    assert(tm->next(tm) == EOF);
    return 1;
}

testcase(test_infinite_letters)
{
    struct txtmac *tm = minit_letter('m', 0);
    assert(tm != NULL);

    for (int i = 0; i < 1000; i++)
        {
            assert(tm->next(tm) == 'm');
        }

    return 1;
};

int main(void)
{
    start_tests();

    run_test(test_one_letter);
    run_test(test_five_letters);
    run_test(test_infinite_letters);

    end_tests();
    return test_result_retcode();
}
