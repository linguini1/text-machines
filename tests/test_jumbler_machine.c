#include <ctype.h>
#include <string.h>

#include "../machines/machines.h"
#include "test.h"

#define JUMBLE_TEST_SEED (90)

static const char long_test[] =
    "This is some test text. It has (?) letters and 3 "
    "numbers, 2 sentences in this 1 buffer!\n";

static const char short_test[] = "Some text, 12345!!\n";

static const char short_test_jumbled[] = "Smoe text, 12345!!\n";

static struct txtmac *long_stream(void)
{
    return minit_buf(long_test, sizeof(long_test));
}

static struct txtmac *short_stream(void)
{
    return minit_buf(short_test, sizeof(short_test));
}

testcase(test_null_stream)
{
    struct txtmac *tm = minit_jumbler(NULL);
    assert(tm == NULL);
    return 1;
}

testcase(test_jumble_short)
{
    char buffer[sizeof(short_test)];

    struct txtmac *stream = short_stream();
    struct txtmac *tm = minit_jumbler(stream);
    assert(tm != NULL);

    /* Jumble content and verify some parts of output */

    sink_buf(tm, buffer, sizeof(buffer));

    /* All non-alpha characters should not have moved */

    for (unsigned i = 0; i < sizeof(short_test); i++)
        {
            if (!isalpha(short_test[i]))
                {
                    assert(buffer[i] == short_test[i]);
                }
        }

    /* First and last letters of words should not have moved */

    assert(buffer[0] == short_test[0]);
    assert(buffer[3] == short_test[3]);
    assert(buffer[5] == short_test[5]);
    assert(buffer[8] == short_test[8]);

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

testcase(test_jumble_long)
{
    char buffer[sizeof(long_test)];

    struct txtmac *stream = long_stream();
    struct txtmac *tm = minit_jumbler(stream);
    assert(tm != NULL);

    /* Jumble content and verify some parts of output */

    sink_buf(tm, buffer, sizeof(buffer));

    /* All non-alpha characters should not have moved */

    for (unsigned i = 0; i < sizeof(short_test); i++)
        {
            if (!isalpha(short_test[i]))
                {
                    assert(buffer[i] == long_test[i]);
                }
        }

    /* First and last letters of words should not have moved */

    assert(buffer[0] == long_test[0]);
    assert(buffer[3] == long_test[3]);
    assert(buffer[80] == long_test[80]);
    assert(buffer[85] == long_test[85]);

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

testcase(test_jumble_short_seeded)
{
    char buffer[sizeof(short_test)];

    struct txtmac *stream = short_stream();
    struct txtmac *tm = minit_jumbler(stream);
    assert(tm != NULL);

    /* Seed random generator */

    srandom(JUMBLE_TEST_SEED);

    /* Jumble content */

    sink_buf(tm, buffer, sizeof(buffer));

    /* Verify seeded content matches previous output */

    if (memcmp(buffer, short_test_jumbled, sizeof(buffer)) != 0)
        {
            mdestroy(stream);
            mdestroy(tm);
            return 0;
        }

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_null_stream);
    run_test(test_jumble_short);
    run_test(test_jumble_long);
    run_test(test_jumble_short_seeded);

    end_tests();
    return test_result_retcode();
}
