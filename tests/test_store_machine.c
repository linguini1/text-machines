/* This test suite is meant to verify the functionality of the "store" text
 * machine, which pre-computes and stores `bufsiz` characters from the previous
 * machine. Both the machine's initialization and output behaviour are tested.
 */
#include <string.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

static char test_text[] = "This is some test text!\n";

static struct txtmac *dummy_stream()
{
    return minit_buf(test_text, sizeof(test_text));
}

/* Verifies that attempted initialization with a NULL stream will return `NULL`
 * as the error case.
 */
testcase(test_null_stream)
{
    struct txtmac *buf = minit_store(NULL, 0);
    assert(buf == NULL);
    return 1;
}

/* Verify that EOF is returned in the error case when the self-reference to
 * 'next' is NULL.
 */
testcase(test_null_selfref)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_store(stream, 16);
    assert(tm != NULL);
    assert(tm->next(NULL) == EOF);
    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verifies that the output of the store machine's next characters match what is
 * in the input stream.
 */
testcase(test_nextchar)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_store(stream, 256);

    assert(tm->next(tm) == 'T');
    assert(tm->next(tm) == 'h');
    assert(tm->next(tm) == 'i');

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verifies that the output of the store text machine (when all of its output is
 * sunk into a buffer) is the same as the original input stream. Parameterized
 * over buffer size.
 */
static int test_sink(size_t bufsiz)
{
    char sinkbuf[sizeof(test_text)];

    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_store(stream, bufsiz);

    sink_buf(tm, sinkbuf, sizeof(sinkbuf));

    /* Compare contents, should be equal */

    if (memcmp(test_text, sinkbuf, sizeof(test_text)) != 0)
        {
            mdestroy(stream);
            mdestroy(tm);
            return 0;
        }

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verifies that the output of the store text machine is the same as the
 * original input stream when no storage buffer is used */
testcase(test_sink_zerostore) { return test_sink(0); }

/* Verifies that the output of the store text machine is the same as the
 * original input stream when a small storage buffer is used */
testcase(test_sink_smallstore) { return test_sink(7); }

/* Verifies that the output of the store text machine is the same as the
 * original input stream when a regular storage buffer is used */
testcase(test_sink_normstore) { return test_sink(BUFSIZ); }

int main(void)
{
    start_tests();

    run_test(test_null_stream);
    run_test(test_null_selfref);
    run_test(test_nextchar);
    run_test(test_sink_zerostore);
    run_test(test_sink_smallstore);
    run_test(test_sink_normstore);

    end_tests();
    return test_result_retcode();
}
