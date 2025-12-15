#include "machines.h"

struct bufmac
{
    struct txtmac tm;
    const char *buf;
    size_t n;
    size_t idx;
};

static char buf_next(struct txtmac *tm)
{
    struct bufmac *priv = (struct bufmac *)tm;

    if (priv->idx < priv->n)
        {
            return priv->buf[priv->idx++];
        }

    return EOF;
}

struct txtmac *minit_buf(const char *buf, size_t n)
{
    struct bufmac *priv = malloc(sizeof(struct bufmac));
    if (priv == NULL) return NULL;

    if (buf == NULL) return NULL;
    priv->buf = buf;
    priv->n = n;
    priv->idx = 0;

    priv->tm.priv = priv;
    priv->tm.next = buf_next;

    return &priv->tm;
}
