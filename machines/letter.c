#include <stdbool.h>

#include "machines.h"

struct letter
{
    struct txtmac tm;
    bool infinite;
    char letter;
    size_t count;
};

static char letter_next(struct txtmac *tm)
{
    struct letter *priv = (struct letter *)tm;

    /* Supply over */

    if (!priv->infinite && priv->count <= 0)
        {
            return EOF;
        }

    priv->count--;
    return priv->letter;
}

struct txtmac *minit_letter(char letter, size_t count)
{
    struct letter *priv = malloc(sizeof(struct letter));
    if (priv == NULL)
        {
            return NULL;
        }

    priv->letter = letter;
    priv->infinite = count == 0 ? true : false;
    priv->count = count;

    priv->tm.priv = priv;
    priv->tm.next = letter_next;

    return &priv->tm;
}
