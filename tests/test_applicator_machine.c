#include "../machines/machines.h"
#include "test.h"
#include <ctype.h>

static const char test_text[] =
    "This is some test text. It has (?) letters and 3 "
    "numbers, 2 sentences in this 1 buffer!\n";

static struct txtmac *dummy_stream(void)
{
    return minit_buf(test_text, sizeof(test_text));
}

static char tolower_c(char c) { return tolower(c); }
static char toupper_c(char c) { return toupper(c); }

testcase(test_lower)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_applicator(stream, tolower_c);

    assert(tm != NULL);

    char c;
    for (unsigned i = 0; i < sizeof(test_text); i++)
        {
            c = tm->next(tm);
            if (isalpha(c)) assert(islower(c));
        }

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

testcase(test_upper)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_applicator(stream, toupper_c);

    assert(tm != NULL);

    char c;
    for (unsigned i = 0; i < sizeof(test_text); i++)
        {
            c = tm->next(tm);
            if (isalpha(c)) assert(isupper(c));
        }

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

testcase(test_null_op)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_applicator(stream, NULL);

    assert(tm == NULL);

    mdestroy(stream);
    return 1;
}

testcase(test_null_stream)
{
    struct txtmac *tm = minit_applicator(NULL, tolower_c);
    assert(tm == NULL);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_lower);
    run_test(test_upper);
    run_test(test_null_op);
    run_test(test_null_stream);

    end_tests();
    return test_result_retcode();
}
