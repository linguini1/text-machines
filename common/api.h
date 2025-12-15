#ifndef _COMMON_API_H_
#define _COMMON_API_H_

#include <stdio.h>

/* Machine APIs */

struct txtmac
{
    /* This function returns the next character to be output by the text
     * machine.
     */
    char (*next)(struct txtmac *);

    /* This member contains private lower-half information from the text machine
     * implementation. It must be allocated using `malloc` and safe to call
     * `free` on without special tear-down logic.
     */
    void *priv;
};

void mdestroy(struct txtmac *tm);

/* Sink options for machine output */

int sink_fd(struct txtmac *tm, int fd);
int sink_file(struct txtmac *tm, FILE *file);
size_t sink_buf(struct txtmac *tm, char *buf, size_t n);

#endif // _COMMON_API_H_
