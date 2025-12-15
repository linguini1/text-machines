#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "api.h"
#include "machines/machines.h"

static char tolower_c(char c) { return tolower(c); }
static char toupper_c(char c) { return toupper(c); }

int main(int argc, char **argv)
{
    /* Open input file to transform */

    FILE *file = fopen("examples/hello.txt", "r");
    if (file == NULL)
        {
            fprintf(stderr, "Couldn't open file: %s\n", strerror(errno));
            return -1;
        }

    /* Create our operations to apply */

    struct txtmac *stream = minit_file(file);
    struct txtmac *tm = minit_applicator(stream, toupper_c);

    /* Perform silly little operations on the text and output the result to the
     * console
     */

    sink_file(tm, stdout);

    mdestroy(stream);
    mdestroy(tm);
    fclose(file);
    return 0;
}
