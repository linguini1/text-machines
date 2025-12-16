#include <ctype.h>

#include "../machines/machines.h"

#define isendpunct(c) (c == '?' || c == '!' || c == '.')

enum state_e
{
    STATE_AFTERPUNC,
    STATE_OUTPUT,
};

struct capital
{
    struct txtmac tm;
    struct txtmac *src;
    enum state_e state;
};

static char capitalize_next(struct txtmac *tm)
{
    if (tm == NULL) return EOF;
    struct capital *priv = (struct capital *)tm;

    char c = priv->src->next(priv->src);

    switch (priv->state)
        {
        case STATE_OUTPUT:
            priv->state = isendpunct(c) ? STATE_AFTERPUNC : STATE_OUTPUT;
            return c;
        case STATE_AFTERPUNC:
            priv->state =
                isendpunct(c) || isspace(c) ? STATE_AFTERPUNC : STATE_OUTPUT;
            if (isalpha(c))
                return toupper(c);
            else
                return c;
        }

    return EOF;
}

struct txtmac *minit_capitalize(struct txtmac *tm)
{
    if (tm == NULL) return NULL;

    struct capital *priv = malloc(sizeof(struct capital));
    if (priv == NULL) return NULL;

    priv->src = tm;
    priv->state = STATE_AFTERPUNC; /* Capitalize beginning of stream */

    priv->tm.priv = priv;
    priv->tm.next = capitalize_next;
    priv->tm.destroy = NULL;

    return &priv->tm;
}
