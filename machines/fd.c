#include <unistd.h>

#include "machines.h"

struct fdmac
{
    struct txtmac tm;
    char buf[BUFSIZ];
    size_t count;
    size_t idx;
    int fd;
};

static char fd_next(struct txtmac *tm)
{
    ssize_t bread;
    struct fdmac *priv = (struct fdmac *)tm;
    if (tm == NULL) return EOF;

    /* Return buffered characters */

    if (priv->idx < priv->count)
        {
            return priv->buf[priv->idx++];
        }

    /* Otherwise read some more characters to buffer */

    bread = read(priv->fd, priv->buf, sizeof(priv->buf));
    if (bread <= 0)
        {
            /* Some error, or we reached the actual EOF anyways. */

            return EOF;
        }
    priv->idx = 0;
    priv->count = bread;

    return priv->buf[priv->idx++];
}

struct txtmac *minit_fd(int fd)
{
    if (fd < 0) return NULL;

    struct fdmac *priv = malloc(sizeof(struct fdmac));
    if (priv == NULL) return NULL;

    priv->fd = fd;
    priv->count = 0;
    priv->idx = 0;

    priv->tm.priv = priv;
    priv->tm.next = fd_next;

    return &priv->tm;
}
