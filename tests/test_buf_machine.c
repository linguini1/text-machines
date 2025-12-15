/* This test suite is meant to verify the functionality of the "buffer" text
 * machine, which outputs the contents of a buffer using the text machine
 * interface. Both the machine's initialization and output behaviour are tested.
 */
#include <string.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

/* Verifies that a buffer text machine initialized using a NULL pointer instead
 * of a true buffer will return `NULL` as the error case.
 */
testcase(test_null_buf)
{
    struct txtmac *buf = minit_buf(NULL, 0);
    assert(buf == NULL);
    return 1;
}

/* Verifies that the output of the buffer text machine `next` function are the
 * characters in the underlying buffer, output in order. Only tests the first
 * few.
 */
testcase(test_full_buf_nextchar)
{
    char testbuf[] = "This is a test buffer.";
    struct txtmac *buf = minit_buf(testbuf, sizeof(testbuf));
    assert(buf != NULL);

    assert(buf->next(buf) == 'T');
    assert(buf->next(buf) == 'h');
    assert(buf->next(buf) == 'i');

    mdestroy(buf);
    return 1;
}

/* Verifies that the output of the buffer text machine (when all of its output
 * is sunk to another buffer) is the same as the contents in the underlying
 * buffer used to initialize it.
 */
testcase(test_full_buf_sink)
{
    char testbuf[] = "This is a test buffer.";
    char sinkbuf[sizeof(testbuf)];

    struct txtmac *buf = minit_buf(testbuf, sizeof(testbuf));
    assert(buf != NULL);

    sink_buf(buf, sinkbuf, sizeof(sinkbuf));

    /* Compare contents, should be equal */

    if (memcmp(testbuf, sinkbuf, sizeof(testbuf) - 1) != 0)
        {
            mdestroy(buf);
            return 0;
        }

    mdestroy(buf);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_null_buf);
    run_test(test_full_buf_nextchar);
    run_test(test_full_buf_sink);

    end_tests();
    return test_result_retcode();
}
