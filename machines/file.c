#include <stdio.h>
#include <stdlib.h>

#include "machines.h"

struct charfile
{
    struct txtmac tm;
    FILE *src;
    char *buf;
    size_t bufsiz;
    size_t count;
    size_t idx;
};

static char file_next(struct txtmac *tm)
{
    struct charfile *priv = (struct charfile *)tm;
    if (tm == NULL) return EOF;

    /* If configured with no buffering, just read a single character */

    if (priv->buf == NULL) return fgetc(priv->src);

    /* If there are buffered contents left, use those */

    if (priv->idx < priv->count)
        {
            return priv->buf[priv->idx++];
        }

    /* Re-fill the buffer if there is nothing left */

    priv->idx = 0;
    priv->count = fread(priv->buf, sizeof(char), priv->bufsiz, priv->src);

    if (priv->count < priv->bufsiz)
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

static void file_destroy(struct txtmac *tm)
{
    struct charfile *priv = (struct charfile *)tm;
    if (priv == NULL) return;

    free(priv->buf);
    priv->buf = NULL;

    priv->src = NULL;
    priv->tm.priv = NULL;
    free(priv);
}

struct txtmac *minit_file(FILE *src, size_t bufsiz)
{
    if (src == NULL) return NULL;

    struct charfile *priv = malloc(sizeof(struct charfile));
    if (priv == NULL) return NULL;

    priv->src = src;
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
    priv->tm.next = file_next;
    priv->tm.destroy = file_destroy;

    return &priv->tm;
}
