/* This test suite is meant to verify the functionality of the "hidden message"
 * text machine, which encodes a hidden message in some input stream. Both the
 * machine's initialization and output behaviour are tested.
 */
#include <string.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

static char test_text[] =
    "This is a super wonderful little piece of text which may or may not "
    "contain a hidden message? Well, actually, this is a canvas for a secret, "
    "hidden message.\n";

static struct txtmac *dummy_stream()
{
    return minit_buf(test_text, sizeof(test_text));
}

/* Verifies that attempted initialization with a NULL stream will return `NULL`
 * as the error case.
 */
testcase(test_null_stream)
{
    struct txtmac *tm = minit_hmsg(NULL, "hi");
    assert(tm == NULL);
    return 1;
}

/* Verifies that attempted initialization with a NULL stream will return `NULL`
 * as the error case.
 */
testcase(test_null_msg)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_hmsg(stream, NULL);
    assert(tm == NULL);
    mdestroy(stream);
    return 1;
}

/* Verify that EOF is returned in the error case when the self-reference to
 * 'next' is NULL.
 */
testcase(test_null_selfref)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_hmsg(stream, "hi");
    assert(tm != NULL);
    assert(tm->next(NULL) == EOF);
    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verifies that the hidden message is encoded in the input
 * stream (first few characters) when it's known in advance that it is
 * achievable.
 */
testcase(test_achieveable_msg_nextchar)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_hmsg(stream, "hello, world");

    assert(tm != NULL);

    assert(tm->next(tm) == 'T');
    assert(tm->next(tm) == 'H');
    assert(tm->next(tm) == 'i');
    assert(tm->next(tm) == 's');

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verifies that the hidden message (in its entirety) is encoded in the input
 * stream when it's known in advance that it is achievable.
 */
testcase(test_achieveable_msg)
{
    char sinkbuf[sizeof(test_text)];
    char expected[] =
        "THis is a supEr wonderfuL Little piece Of text Which may OR may not "
        "contain a hidden message? WeLl, actually, this is a canvas for a "
        "secret, hiDden message.\n";

    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_hmsg(stream, "hello, world");
    assert(tm != NULL);

    sink_buf(tm, sinkbuf, sizeof(sinkbuf));

    /* Compare contents, should be equal */

    if (memcmp(sinkbuf, expected, sizeof(expected)) != 0)
        {
            mdestroy(stream);
            mdestroy(tm);
            return 0;
        }

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verifies that the hidden message is partially encoded in the input
 * stream with the remainder appended when the input stream does not have the
 * sufficient characters to encode the hidden message.
 */
testcase(test_unachieveable_msg)
{
    char expected[] =
        "THIs is a super wonderful little piece of text which may or may not "
        "contain a hidden message? Well, actually, this is a canvas for a "
        "secret, hidden message.\n\0zebra";
    char sinkbuf[sizeof(expected)];

    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_hmsg(stream, "hi, zebra");
    assert(tm != NULL);

    sink_buf(tm, sinkbuf, sizeof(sinkbuf));

    /* Compare contents, should be equal */

    if (memcmp(sinkbuf, expected, sizeof(expected)) != 0)
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
    run_test(test_null_msg);
    run_test(test_null_selfref);
    run_test(test_achieveable_msg_nextchar);
    run_test(test_achieveable_msg);
    run_test(test_unachieveable_msg);

    end_tests();
    return test_result_retcode();
}
