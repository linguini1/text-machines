#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "api.h"
#include "machines/machines.h"

#define nullcheck(p, msg)                                                      \
    do                                                                         \
        {                                                                      \
            if ((p) == NULL)                                                   \
                {                                                              \
                    fprintf(stderr, msg);                                      \
                }                                                              \
        }                                                                      \
    while (0)

static char tolower_c(char c) { return tolower(c); }
static char toupper_c(char c) { return toupper(c); }

int main(int argc, char **argv)
{
    /* Turn the input file into a generative text machine */

    FILE *file = fopen("examples/pig.txt", "r");
    if (file == NULL)
        {
            fprintf(stderr, "Couldn't open file: %s\n", strerror(errno));
            return -1;
        }

    struct txtmac *stream = minit_file(file);
    nullcheck(stream, "Couldn't init file stream text machine.\n");

    /* Create our operations to apply */

    struct txtmac *tm = minit_jumbler(stream);
    nullcheck(tm, "Couldn't init jumbler text machine.\n");
    struct txtmac *upper = minit_applicator(tm, toupper_c);
    nullcheck(upper, "Couldn't init applicator text machine.\n");

    /* Perform silly little operations on the text and output the result to the
     * console
     */

    sink_file(upper, stdout);

    mdestroy(stream);
    mdestroy(tm);
    mdestroy(upper);
    fclose(file);
    return 0;
}
