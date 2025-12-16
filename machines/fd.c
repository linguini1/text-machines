#include <unistd.h>

#include "machines.h"

struct fdmac
{
    struct txtmac tm;
    char *buf;
    size_t bufsiz;
    size_t count;
    size_t idx;
    int fd;
};

static char fd_next(struct txtmac *tm)
{
    ssize_t bread;
    char c;
    struct fdmac *priv = (struct fdmac *)tm;
    if (tm == NULL) return EOF;

    /* If configured with no buffering, just read a single character. */

    if (priv->buf == NULL)
        {
            bread = read(priv->fd, &c, sizeof(c));
            if (bread <= 0)
                {
                    /* Error or end of file */
                    return EOF;
                }
            return c;
        }

    /* Return buffered characters */

    if (priv->idx < priv->count)
        {
            return priv->buf[priv->idx++];
        }

    /* Otherwise read some more characters to buffer */

    bread = read(priv->fd, priv->buf, priv->bufsiz);
    if (bread <= 0)
        {
            /* Some error, or we reached the actual EOF anyways. */

            return EOF;
        }
    priv->idx = 0;
    priv->count = bread;

    return priv->buf[priv->idx++];
}

static void fd_destroy(struct txtmac *tm)
{
    struct fdmac *priv = (struct fdmac *)tm;
    if (priv == NULL) return;

    free(priv->buf);
    priv->buf = NULL;

    priv->fd = -1;
    priv->tm.priv = NULL;
    free(priv);
}

struct txtmac *minit_fd(int fd, size_t bufsiz)
{
    if (fd < 0) return NULL;

    struct fdmac *priv = malloc(sizeof(struct fdmac));
    if (priv == NULL) return NULL;

    priv->fd = fd;
    priv->count = 0;
    priv->idx = 0;
    priv->bufsiz = bufsiz;

    /* Allocate buffer */

    if (bufsiz == 0)
        {
            priv->buf = NULL;
        }
    else
        {
            priv->buf = malloc(bufsiz * sizeof(char));
            if (priv->buf == NULL)
                {
                    free(priv);
                    return NULL;
                }
        }

    priv->tm.priv = priv;
    priv->tm.next = fd_next;
    priv->tm.destroy = fd_destroy;

    return &priv->tm;
}
