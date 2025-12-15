#include <errno.h>
#include <unistd.h>

#include "api.h"

int sink_fd(struct txtmac *tm, int fd)
{
    char buffer[BUFSIZ];
    ssize_t bytes;
    if (tm == NULL) return EINVAL;
    if (fd < 0) return EINVAL;

    for (;;)
        {
            bytes = sink_buf(tm, buffer, sizeof(buffer));
            bytes = write(fd, buffer, bytes);
            if (bytes < 0)
                {
                    return errno;
                }
            else if (bytes == 0)
                {
                    return 0;
                }
        }
}

int sink_file(struct txtmac *tm, FILE *file)
{
    char buffer[BUFSIZ];
    size_t bytes;
    if (tm == NULL) return EINVAL;
    if (file == NULL) return EINVAL;

    for (;;)
        {
            bytes = sink_buf(tm, buffer, sizeof(buffer));
            bytes = fwrite(buffer, sizeof(char), bytes, file);
            if (bytes < sizeof(buffer))
                {
                    if (ferror(file))
                        {
                            return errno;
                        }

                    /* We didn't have a full buffer, which only happens when the
                     * text machine has sent an EOF.
                     */

                    return 0;
                }
        }
}

size_t sink_buf(struct txtmac *tm, char *buf, size_t n)
{
    size_t i;
    if (tm == NULL) return 0;
    if (buf == NULL) return 0;

    for (i = 0; i < n; i++)
        {
            buf[i] = tm->next(tm);
            if (buf[i] == EOF)
                {
                    buf[i] = '\0';
                    break;
                }
        }

    /* Return bytes written */

    return i + 1;
}
