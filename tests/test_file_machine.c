/* This test suite is meant to verify the functionality of the "file" text
 * machine, which outputs the contents of a file pointed to by a file pointer
 * using the text machine interface. Both the machine's initialization and
 * output behaviour are tested.
 */
#include <stdio.h>
#include <string.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

/* Verifies that NULL is returned in the error case when the file pointer is
 * NULL.
 */
testcase(test_invalid_file)
{
    struct txtmac *filemac = minit_file(NULL, 0);
    assert(filemac == NULL);
    return 1;
}

/* Verifies that the first few characters of the text machine output match the
 * characters in the backing file.
 */
testcase(test_helloworld_file)
{
    FILE *file = fopen("examples/hello.txt", "r");
    assert(file != NULL);

    struct txtmac *fmac = minit_file(file, BUFSIZ);
    assert(fmac != NULL);

    assert(fmac->next(fmac) == 'H');
    assert(fmac->next(fmac) == 'e');
    assert(fmac->next(fmac) == 'l');

    mdestroy(fmac);
    fclose(file);
    return 1;
}

/* Parameterizes the hello world file sink test with a buffer size. */

static int helloworld_file_sink(size_t bufsiz)
{
    int res = 1;
    char buffer[64];
    char fbuffer[64];
    size_t bread;
    FILE *file = fopen("examples/hello.txt", "r");
    assert(file != NULL);

    struct txtmac *fmac = minit_file(file, bufsiz);
    assert(fmac != NULL);

    /* Gather text machine output */

    sink_buf(fmac, buffer, sizeof(buffer));

    /* Rewind file to the beginning to read again */

    if (fseek(file, 0, SEEK_SET) < 0)
        {
            res = 0;
            goto early_ret;
        }

    /* Get file contents in a buffer */

    bread = fread(fbuffer, sizeof(char), sizeof(fbuffer), file);
    if (bread <= 0)
        {
            res = 0;
            goto early_ret;
        }

    /* Compare buffer contents */

    if (memcmp(fbuffer, buffer, bread) != 0)
        {
            res = 0;
            goto early_ret;
        }

early_ret:
    mdestroy(fmac);
    fclose(file);
    return res;
}

/* Verifies that the first few characters of the text machine output match the
 * characters in the backing file.
 */
testcase(test_helloworld_file_sink) { return helloworld_file_sink(BUFSIZ); }

/* Verifies that the first few characters of the text machine output match the
 * characters in the backing file when no buffering is used.
 */
testcase(test_helloworld_file_sink_nobuf) { return helloworld_file_sink(0); }

/* Verifies that the first few characters of the text machine output match the
 * characters in the backing file when a small buffer is used.
 */
testcase(test_helloworld_file_sink_smallbuf) { return helloworld_file_sink(7); }

/* Verify that EOF is returned in the error case when the self-reference to
 * 'next' is NULL.
 */
testcase(test_null_selfref)
{
    FILE *file = fopen("examples/hello.txt", "r");
    assert(file != NULL);

    struct txtmac *fmac = minit_file(file, 0);
    assert(fmac != NULL);

    assert(fmac->next(NULL) == EOF);

    mdestroy(fmac);
    fclose(file);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_invalid_file);
    run_test(test_helloworld_file);
    run_test(test_helloworld_file_sink);
    run_test(test_helloworld_file_sink_nobuf);
    run_test(test_helloworld_file_sink_smallbuf);
    run_test(test_null_selfref);

    end_tests();
    return test_result_retcode();
}
