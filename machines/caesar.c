#include <ctype.h>

#include "../machines/machines.h"

struct caesar
{
    struct txtmac tm;
    struct txtmac *src;
    unsigned shift;
};

static char caesar_next(struct txtmac *tm)
{
    struct caesar *priv = (struct caesar *)tm;
    if (priv == NULL) return EOF;

    char c = priv->src->next(priv->src);

    if (!isalpha(c)) return c;

    /* Alphabetic characters get shifted */

    if (isupper(c))
        {
            return toupper(((tolower(c) - 'a' + priv->shift) % 26) + 'a');
        }

    return ((c - 'a' + priv->shift) % 26) + 'a';
}

struct txtmac *minit_caesar(struct txtmac *tm, unsigned shift)
{
    if (tm == NULL) return NULL;
    if (shift > 25) return NULL;
    struct caesar *priv = malloc(sizeof(struct caesar));
    if (priv == NULL) return NULL;

    priv->src = tm;
    priv->shift = shift;

    priv->tm.priv = priv;
    priv->tm.next = caesar_next;
    priv->tm.destroy = NULL;

    return &priv->tm;
}
