#include <string.h>

#include "../machines/machines.h"
#include "test.h"

static const char one_word[] = "Balloon.\n";

static const char long_text[] =
    "This is some test text. It has (?) letters and 3 "
    "numbers, 2 sentences in this 1 buffer!\n";

static struct txtmac *oneword_stream(void)
{
    return minit_buf(one_word, sizeof(one_word));
}

static struct txtmac *long_stream(void)
{
    return minit_buf(long_text, sizeof(long_text));
}

testcase(test_null_stream)
{
    struct txtmac *tm = minit_replace(NULL, "hi", "hello");
    assert(tm == NULL);
    return 1;
}

testcase(test_null_searchterm)
{
    struct txtmac *tm = minit_replace((struct txtmac *)12, NULL, "hello");
    assert(tm == NULL);
    return 1;
}

testcase(test_empty_searchterm)
{
    char buffer[sizeof(one_word)];
    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "", "a");

    /* Should result in no change on the string */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, one_word) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_one_letter_replace)
{
    static const char expected[] = "Ballxxn.\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "o", "x");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_multi_letter_replace)
{
    static const char expected[] = "Ballxn.\n";
    char buffer[sizeof(one_word)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "oo", "x");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_one_letter_replace_multi)
{
    static const char expected[] = "Balloooon.\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "o", "oo");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_one_letter_remove)
{
    static const char expected[] = "Baoon.\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "l", NULL);

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_multi_letter_remove)
{
    static const char expected[] = "Ball.\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "oon", NULL);

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_one_letter_remove_emptystr)
{
    static const char expected[] = "Baoon.\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "l", "");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_multi_letter_remove_emptystr)
{
    static const char expected[] = "Ball.\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = oneword_stream();
    struct txtmac *tm = minit_replace(stream, "oon", "");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_long_remove_is)
{
    static const char expected[] =
        "Th  some test text. It has (?) letters and 3 "
        "numbers, 2 sentences in th 1 buffer!\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = long_stream();
    struct txtmac *tm = minit_replace(stream, "is", NULL);

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_long_remove_is_emptystr)
{
    static const char expected[] =
        "Th  some test text. It has (?) letters and 3 "
        "numbers, 2 sentences in th 1 buffer!\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = long_stream();
    struct txtmac *tm = minit_replace(stream, "is", "");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_long_remove_word)
{
    static const char expected[] =
        "This is some test text. It has (?) letters and 3 "
        "numbers, 2  in this 1 buffer!\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = long_stream();
    struct txtmac *tm = minit_replace(stream, "sentences", NULL);

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

testcase(test_long_replace_word)
{
    static const char expected[] =
        "This is some test text. It has (?) letters and 3 "
        "numbers, 2 beagles in this 1 buffer!\n";
    char buffer[sizeof(expected)];

    struct txtmac *stream = long_stream();
    struct txtmac *tm = minit_replace(stream, "sentences", "beagles");

    /* Compare against expected output */

    sink_buf(tm, buffer, sizeof(buffer));
    if (strcmp(buffer, expected) != 0)
        {
            mdestroy(tm);
            mdestroy(stream);
            return 0;
        }

    mdestroy(tm);
    mdestroy(stream);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_null_stream);
    run_test(test_null_searchterm);
    run_test(test_empty_searchterm);
    run_test(test_one_letter_replace);
    run_test(test_multi_letter_replace);
    run_test(test_one_letter_replace_multi);
    run_test(test_one_letter_remove);
    run_test(test_multi_letter_remove);
    run_test(test_one_letter_remove_emptystr);
    run_test(test_multi_letter_remove_emptystr);

    run_test(test_long_remove_is);
    run_test(test_long_remove_is_emptystr);
    run_test(test_long_remove_word);
    run_test(test_long_replace_word);

    end_tests();
    return test_result_retcode();
}
