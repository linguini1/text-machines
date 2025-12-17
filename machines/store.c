#include "api.h"
#include "machines.h"

struct store
{
    struct txtmac tm;
    struct txtmac *src;
    char *buf;
    size_t bufsiz;
    size_t count;
    size_t idx;
};

static char store_next(struct txtmac *tm)
{
    struct store *priv = (struct store *)tm;
    if (priv == NULL) return EOF;

    /* If we don't do any storage, just send the next character along */

    if (priv->buf == NULL) return priv->src->next(priv->src);

    /* If we need to update the store of characters, do that */

    if (priv->idx >= priv->count)
        {
            priv->count = 0;
            while (priv->count < priv->bufsiz)
                {
                    priv->buf[priv->count++] = priv->src->next(priv->src);
                    if (priv->buf[priv->count - 1] == EOF) break;
                }
            priv->idx = 0;
        }

    /* If we have characters that we can output, output them */

    if (priv->idx < priv->count)
        {
            return priv->buf[priv->idx++];
        }

    return EOF;
}

static void store_destroy(struct txtmac *tm)
{
    struct store *priv = (struct store *)tm;
    if (priv == NULL) return;

    /* Free the buffer and destroy all references */

    free(priv->buf);
    priv->buf = NULL;
    priv->src = NULL;
    return free(priv);
}

struct txtmac *minit_store(struct txtmac *tm, size_t bufsiz)
{
    if (tm == NULL) return NULL;
    struct store *priv = malloc(sizeof(struct store));
    if (priv == NULL) return NULL;

    priv->src = tm;
    priv->bufsiz = bufsiz;
    priv->idx = 0;
    priv->count = 0;

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
    priv->tm.next = store_next;
    priv->tm.destroy = store_destroy;

    return &priv->tm;
}
