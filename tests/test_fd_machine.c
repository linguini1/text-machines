/* This test suite is meant to verify the functionality of the "fd" text
 * machine, which outputs the contents of a file pointed to by `fd` using the
 * text machine interface. Both the machine's initialization and output
 * behaviour are tested.
 */
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

/* Verifies that NULL is returned in the error case when the file descriptor is
 * negative.
 */
testcase(test_invalid_fd)
{
    struct txtmac *fdmac = minit_fd(-1, 0);
    assert(fdmac == NULL);
    return 1;
}

/* Verifies that the first few letters returned by the text machine match the
 * letters in the underlying text file.
 */
testcase(test_helloworld_fd)
{
    int fd = open("examples/hello.txt", O_RDONLY);
    assert(fd >= 0);

    struct txtmac *fdmac = minit_fd(fd, BUFSIZ);
    assert(fdmac != NULL);

    assert(fdmac->next(fdmac) == 'H');
    assert(fdmac->next(fdmac) == 'e');
    assert(fdmac->next(fdmac) == 'l');

    mdestroy(fdmac);
    close(fd);
    return 1;
}

/* Parameterizes the hello world sink test with a buffer size */

static int helloworld_sink(size_t bufsiz)
{
    char buffer[64];
    char fbuffer[64];
    ssize_t bread;
    int res = 1;

    int fd = open("examples/hello.txt", O_RDONLY);
    assert(fd >= 0);

    struct txtmac *fdmac = minit_fd(fd, bufsiz);
    assert(fdmac != NULL);

    /* Gather text machine output */

    sink_buf(fdmac, buffer, sizeof(buffer));

    /* Rewind file to the beginning to read again */

    if (lseek(fd, 0, SEEK_SET) < 0)
        {
            res = 0;
            goto early_ret;
        }

    /* Get file contents in a buffer */

    bread = read(fd, fbuffer, sizeof(fbuffer));
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
    mdestroy(fdmac);
    close(fd);
    return res;
}

/* Verifies that the entire output of the text machine matches the exact
 * contents of the underlying text file.
 */
testcase(test_helloworld_sink) { return helloworld_sink(BUFSIZ); }

/* Verifies that the entire output of the text machine matches the exact
 * contents of the underlying text file when no buffering is used.
 */
testcase(test_helloworld_sink_nobuf) { return helloworld_sink(0); }

/* Verifies that the entire output of the text machine matches the exact
 * contents of the underlying text file when a small buffer is used.
 */
testcase(test_helloworld_sink_smallbuf) { return helloworld_sink(7); }

/* Verify that EOF is returned in the error case when the self-reference to
 * 'next' is NULL.
 */
testcase(test_null_selfref)
{
    int fd = open("examples/hello.txt", O_RDONLY);
    assert(fd >= 0);

    struct txtmac *fdmac = minit_fd(fd, 0);
    assert(fdmac != NULL);

    assert(fdmac->next(NULL) == EOF);

    mdestroy(fdmac);
    close(fd);
    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_invalid_fd);
    run_test(test_helloworld_fd);
    run_test(test_helloworld_sink);
    run_test(test_helloworld_sink_nobuf);
    run_test(test_helloworld_sink_smallbuf);
    run_test(test_null_selfref);

    end_tests();
    return test_result_retcode();
}
