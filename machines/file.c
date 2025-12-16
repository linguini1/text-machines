#include <stdio.h>
#include <stdlib.h>

#include "machines.h"

struct charfile
{
    struct txtmac tm;
    FILE *src;
    char buf[BUFSIZ];
    size_t count;
    size_t idx;
};

static char file_next(struct txtmac *tm)
{
    struct charfile *priv = (struct charfile *)tm;
    if (tm == NULL) return EOF;

    /* If there are buffered contents left, use those */

    if (priv->idx < priv->count)
        {
            return priv->buf[priv->idx++];
        }

    /* Re-fill the buffer if there is nothing left */

    priv->idx = 0;
    priv->count = fread(priv->buf, sizeof(char), sizeof(priv->buf), priv->src);

    if (priv->count < sizeof(priv->buf))
        {
            if (ferror(priv->src))
                {
                    return EOF;
                }
            else if (feof(priv->src))
                {
                    priv->buf[priv->count] = EOF;
                    priv->count++;
                }
        }

    return priv->buf[priv->idx++];
}

struct txtmac *minit_file(FILE *src)
{
    if (src == NULL) return NULL;

    struct charfile *priv = malloc(sizeof(struct charfile));
    if (priv == NULL) return NULL;

    priv->src = src;
    priv->count = 0;
    priv->idx = 0;

    priv->tm.priv = priv;
    priv->tm.next = file_next;
    priv->tm.destroy = NULL;

    return &priv->tm;
}
