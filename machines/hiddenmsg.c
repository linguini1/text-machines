#include <ctype.h>
#include <stdbool.h>

#include "machines.h"

struct hmsg
{
    struct txtmac tm;
    struct txtmac *src;
    const char *msg;
    size_t idx;
    bool append;
};

static char hmsg_next(struct txtmac *tm)
{
    char c;
    struct hmsg *priv = (struct hmsg *)tm;
    if (priv == NULL) return EOF;

    c = priv->src->next(priv->src);

    /* Ignore non-alphabetical characters in the message */

    while (!isalpha(priv->msg[priv->idx]) && priv->msg[priv->idx] != '\0')
        priv->idx++;

    /* Check if we hit the end of the input stream before all of the message
     * could be encoded. */

    if (!priv->append)
        {
            priv->append = (c == EOF && priv->msg[priv->idx] != '\0');
        }

    if (priv->append)
        {
            /* Start appending to the end of the output */

            if (priv->msg[priv->idx] != '\0') return priv->msg[priv->idx++];

            return EOF;
        }

    /* Not a match for our hidden message */

    if (tolower(c) != tolower(priv->msg[priv->idx])) return c;

    /* We have a match, make sure the message can be distinguished in the
     * output. */

    priv->idx++;
    if (islower(c))
        {
            return toupper(c);
        }
    else
        {
            return tolower(c);
        }
}

struct txtmac *minit_hmsg(struct txtmac *tm, const char *msg)
{
    if (tm == NULL) return NULL;
    if (msg == NULL) return NULL;

    struct hmsg *priv = malloc(sizeof(struct hmsg));
    if (priv == NULL) return NULL;

    priv->src = tm;
    priv->msg = msg;
    priv->idx = 0;
    priv->append = false;

    priv->tm.priv = priv;
    priv->tm.next = hmsg_next;
    priv->tm.destroy = NULL;

    return &priv->tm;
}
