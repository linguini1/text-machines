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
    struct txtmac *filemac = minit_file(NULL);
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

    struct txtmac *fmac = minit_file(file);
    assert(fmac != NULL);

    assert(fmac->next(fmac) == 'H');
    assert(fmac->next(fmac) == 'e');
    assert(fmac->next(fmac) == 'l');

    mdestroy(fmac);
    fclose(file);
    return 1;
}

/* Verifies that the first few characters of the text machine output match the
 * characters in the backing file.
 */
testcase(test_helloworld_file_sink)
{
    int res = 1;
    char buffer[64];
    char fbuffer[64];
    size_t bread;
    FILE *file = fopen("examples/hello.txt", "r");
    assert(file != NULL);

    struct txtmac *fmac = minit_file(file);
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

int main(void)
{
    start_tests();

    run_test(test_invalid_file);
    run_test(test_helloworld_file);
    run_test(test_helloworld_file_sink);

    end_tests();
    return test_result_retcode();
}
