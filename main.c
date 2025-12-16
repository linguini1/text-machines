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
                    exit(EXIT_FAILURE);                                        \
                }                                                              \
        }                                                                      \
    while (0)

int main(int argc, char **argv)
{
    FILE *file = stdin;

    /* We got a file path as the first argument, use that as input */

    if (argc == 2)
        {
            file = fopen(argv[1], "r");
            if (file == NULL)
                {
                    fprintf(stderr, "Couldn't open file: %s\n",
                            strerror(errno));
                    return EXIT_FAILURE;
                }
        }

    /* Too many arguments */

    else if (argc > 2)
        {
            fprintf(stderr, "Usage: %s [path/to/input/file.txt]\n", argv[0]);
            return EXIT_FAILURE;
        }

    /* Turn the input file into a generative text machine */

    struct txtmac *stream = minit_file(file, BUFSIZ);
    nullcheck(stream, "Couldn't init file stream text machine.\n");

    /* Create our operations to apply */

    struct txtmac *tm = minit_replace(stream, "the", "Thee");

    /* Perform silly little operations on the text and output the result to the
     * console
     */

    sink_file(tm, stdout);

    /* Clean up */

    mdestroy(stream);
    mdestroy(tm);

    if (file != stdin)
        {
            fclose(file);
        }

    return EXIT_SUCCESS;
}
