/* This test suite is meant to verify the functionality of the "caesar" text
 * machine, which applies the Caesar cipher to alphabetic characters in the
 * text. Both the machine's initialization and output behaviour are tested.
 */
#include <string.h>

#include "../machines/machines.h"
#include "test.h"

static const char test_text[] =
    "I'm going to Caesar Cipher this text, and it will maintain the "
    "capitalization, spaces and punctuation.\n";

static struct txtmac *dummy_stream(void)
{
    return minit_buf(test_text, sizeof(test_text));
}

/* Verify that in the error case of passing a NULL input stream, the returned
 * text machine is also NULL.
 */
testcase(test_null_stream)
{
    struct txtmac *tm = minit_caesar(NULL, 0);
    assert(tm == NULL);
    return 1;
}

/* Verify that in the error case of passing a shift that is too high, the
 * returned text machine is NULL.
 */
testcase(test_invalid_shift)
{
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_caesar(stream, 29);
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
    struct txtmac *tm = minit_caesar(stream, 3);

    assert(tm->next(NULL) == EOF);

    mdestroy(stream);
    mdestroy(tm);
    return 1;
}

/* Verify that all of the letters in the sample text are capitalized.
 */
testcase(test_cipher_shift_3)
{
    int res = 1;
    const char expected[] =
        "L'p jrlqj wr Fdhvdu Flskhu wklv whaw, dqg lw zloo pdlqwdlq wkh "
        "fdslwdolcdwlrq, vsdfhv dqg sxqfwxdwlrq.\n";
    char buffer[sizeof(expected)];
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_caesar(stream, 3);

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

/* Verify that a shift of 0 does not modify the original text.
 */
testcase(test_cipher_zero_shift)
{
    int res = 1;
    char buffer[sizeof(test_text)];
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_caesar(stream, 0);

    assert(tm != NULL);

    sink_buf(tm, buffer, sizeof(buffer));
    if (memcmp(buffer, test_text, sizeof(test_text)) != 0)
        {
            res = 0;
        }

    mdestroy(stream);
    mdestroy(tm);
    return res;
}

/* Verify that the maximum shift number encrypts the text properly.
 */
testcase(test_cipher_max_shift)
{
    int res = 1;
    const char expected[] =
        "H'l fnhmf sn Bzdrzq Bhogdq sghr sdws, zmc hs vhkk lzhmszhm sgd "
        "bzohszkhyzshnm, rozbdr zmc otmbstzshnm.\n";
    char buffer[sizeof(expected)];
    struct txtmac *stream = dummy_stream();
    struct txtmac *tm = minit_caesar(stream, 25);

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

/* Verify that consecutive shifts which put 'A' back to the 0th index (shifts
 * that add up to 0 mod 26) decode text back to the original.
 */
testcase(test_cipher_decode)
{
    int res = 1;
    char buffer[sizeof(test_text)];
    struct txtmac *stream = dummy_stream();
    struct txtmac *encode = minit_caesar(stream, 5);
    struct txtmac *tm = minit_caesar(encode, 21);

    assert(tm != NULL);

    sink_buf(tm, buffer, sizeof(buffer));
    if (memcmp(buffer, test_text, sizeof(test_text)) != 0)
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
    run_test(test_invalid_shift);
    run_test(test_null_selfref);
    run_test(test_cipher_shift_3);
    run_test(test_cipher_zero_shift);
    run_test(test_cipher_max_shift);
    run_test(test_cipher_decode);

    end_tests();
    return test_result_retcode();
}
