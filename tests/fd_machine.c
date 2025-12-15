#include <fcntl.h>
#include <unistd.h>

#include "../machines/machines.h"
#include "api.h"
#include "test.h"

testcase(test_invalid_fd)
{
    struct txtmac *fdmac = minit_fd(-1);
    assert(fdmac == NULL);
    return 1;
}

testcase(test_helloworld_fd)
{
    int fd = open("examples/hello.txt", O_RDONLY);
    assert(fd >= 0);

    struct txtmac *fdmac = minit_fd(fd);
    assert(fdmac != NULL);

    assert(fdmac->next(fdmac) == 'H');
    assert(fdmac->next(fdmac) == 'e');
    assert(fdmac->next(fdmac) == 'l');

    return 1;
}

int main(void)
{
    start_tests();

    run_test(test_invalid_fd);
    run_test(test_helloworld_fd);

    end_tests();
    return test_result_retcode();
}
