#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "api.h"
#include "machines/machines.h"

static char tolower_c(char c) { return tolower(c); }
static char toupper_c(char c) { return toupper(c); }

int main(int argc, char **argv)
{
    /* Turn the input file into a generative text machine */

#if 0
    FILE *file = fopen("examples/pig.txt", "r");
    if (file == NULL)
        {
            fprintf(stderr, "Couldn't open file: %s\n", strerror(errno));
            return -1;
        }

    struct txtmac *stream = minit_file(file);
#endif

    int fd = open("examples/pig.txt", O_RDONLY);
    if (fd < 0)
        {
            fprintf(stderr, "Couldn't open file: %s\n", strerror(errno));
        }
    struct txtmac *stream = minit_fd(fd);

    /* Create our operations to apply */

    struct txtmac *tm = minit_jumbler(stream);

    /* Perform silly little operations on the text and output the result to the
     * console
     */

    sink_file(tm, stdout);

    mdestroy(stream);
    mdestroy(tm);
    close(fd);
    return 0;
}
