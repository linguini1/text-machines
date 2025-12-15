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
    struct txtmac *fdmac = minit_fd(-1);
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

    struct txtmac *fdmac = minit_fd(fd);
    assert(fdmac != NULL);

    assert(fdmac->next(fdmac) == 'H');
    assert(fdmac->next(fdmac) == 'e');
    assert(fdmac->next(fdmac) == 'l');

    mdestroy(fdmac);
    close(fd);
    return 1;
}

/* Verifies that the entire output of the text machine matches the exact
 * contents of the underlying text file.
 */
testcase(test_helloworld_sink)
{
    char buffer[64];
    char fbuffer[64];
    ssize_t bread;
    int res = 1;

    int fd = open("examples/hello.txt", O_RDONLY);
    assert(fd >= 0);

    struct txtmac *fdmac = minit_fd(fd);
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

int main(void)
{
    start_tests();

    run_test(test_invalid_fd);
    run_test(test_helloworld_fd);
    run_test(test_helloworld_sink);

    end_tests();
    return test_result_retcode();
}
