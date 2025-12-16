/* This test suite is meant to verify the functionality of the "capitalize" text
 * machine, which capitalizes the start of every word. Both the machine's
 * initialization and output behaviour are tested.
 */
#include <string.h>

#include "../machines/machines.h"
#include "test.h"

static const char test_text[] =
    "this is some test text. it has (?) letters and 3 "
    "numbers, 2 sentences in this 1 buffer!\n";

static const char test_text_2[] =
    "is this is some test text...? nobody knows.   except me, of course!\n";

static struct txtmac *dummy_stream(void)
{
    return minit_buf(test_text, sizeof(test_text));
}

static struct txtmac *dummy_stream2(void)
{
    return minit_buf(test_text_2, sizeof(test_text_2));
}

/* Verify that in the error case of passing a NULL input stream, the returned
 * text machine is also NULL.
 */
testcase(test_null_stream)
{
    struct txtmac *tm = minit_capitalize(NULL);
    assert(tm == NULL);
    return 1;
}

/* Verify that EOF is returned in the error case when the self-reference to
 * 'next' is NULL.
 */
testcase(test_null_selfref)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_capitalize(stream);

    assert(tm->next(NULL) == EOF);

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verify that all of the letters in the sample text are capitalized.
 */
testcase(test_capitalization)
{
    int res = 1;
    const char expected[] = "This is some test text. It has (?) letters and 3 "
                            "numbers, 2 sentences in this 1 buffer!\n";
    char buffer[sizeof(expected)];
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_capitalize(stream);

    assert(tm != NULL);

    sink_buf(tm, buffer, sizeof(buffer));
    if (memcmp(buffer, expected, sizeof(expected)) != 0)
        {
            res = 0;
        }

    mdestroy(stream);
    mdestroy(tm);
    return res;
}

/* Verify that all of the letters in the sample text are capitalized, even when
 * there are multiple punctuation points and uneven whitespace.
 */
testcase(test_capitalization_multi)
{
    int res = 1;
    const char expected[] =
        "Is this is some test text...? Nobody knows.   Except me, of course!\n";

    char buffer[sizeof(expected)];
    struct txtmac *stream = dummy_stream2();
    struct txtmac *tm = minit_capitalize(stream);

    assert(tm != NULL);

    sink_buf(tm, buffer, sizeof(buffer));
    if (memcmp(buffer, expected, sizeof(expected)) != 0)
        {
            res = 0;
        }

    mdestroy(stream);
    mdestroy(tm);
    return res;
}

int main(void)
{
    start_tests();

    run_test(test_null_stream);
    run_test(test_null_selfref);
    run_test(test_capitalization);
    run_test(test_capitalization_multi);

    end_tests();
    return test_result_retcode();
}
