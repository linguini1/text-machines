/* This test suite is meant to verify the functionality of the "letter" text
 * machine, which outputs a single letter `count` times (or infinitely if count
 * is 0). Both the machine's initialization and output behaviour are tested.
 */
#include "../machines/machines.h"
#include "test.h"

/* Verify that parameters ('a', 1) result in the letter 'a' being output once,
 * followed by an EOF character.
 */
testcase(test_one_letter)
{
    struct txtmac *tm = minit_letter('a', 1);
    assert(tm != NULL);

    assert(tm->next(tm) == 'a');
    assert(tm->next(tm) == EOF);
    mdestroy(tm);
    return 1;
}

/* Verify that parameters ('m', 5) result in the letter 'm' being output five
 * times, followed by an EOF character.
 */
testcase(test_five_letters)
{
    struct txtmac *tm = minit_letter('m', 5);
    assert(tm != NULL);

    for (int i = 0; i < 5; i++)
        {
            assert(tm->next(tm) == 'm');
        }

    assert(tm->next(tm) == EOF);
    mdestroy(tm);
    return 1;
}

/* Verify that parameters ('m', 0) result in the letter 'm' being output
 * "infinite" times (check 1000 `next` calls).
 */
testcase(test_infinite_letters)
{
    struct txtmac *tm = minit_letter('m', 0);
    assert(tm != NULL);

    for (int i = 0; i < 1000; i++)
        {
            assert(tm->next(tm) == 'm');
        }

    mdestroy(tm);
    return 1;
};

/* Verify that EOF is returned in the error case when the self-reference to
 * 'next' is NULL.
 */
testcase(test_null_selfref)
{
    struct txtmac *tm = minit_letter('m', 0);
    assert(tm != NULL);
    assert(tm->next(NULL) == EOF);
    mdestroy(tm);
    return 1;
};

int main(void)
{
    start_tests();

    run_test(test_one_letter);
    run_test(test_five_letters);
    run_test(test_infinite_letters);
    run_test(test_null_selfref);

    end_tests();
    return test_result_retcode();
}
