#include <ctype.h>
#include <stdbool.h>

#include "machines.h"

#define JUMBLE_BUFSIZ (128)

enum state_e
{
    STATE_START,
    STATE_ALPHA_FIRST,
    STATE_ALPHA_N,
    STATE_NONALPHA,
    STATE_JUMBLE,
};

struct jumbler
{
    struct txtmac tm;
    struct txtmac *src;
    enum state_e state;
    char lastchar;
    char buf[JUMBLE_BUFSIZ];
    size_t bufidx;
};

static void shuffle(char *arr, size_t n)
{
    /* Nothing to shuffle. */

    if (n <= 1) return;

    /* Shuffle the characters. */

    size_t i;
    for (i = 0; i < n; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            char t = arr[j];
            arr[j] = arr[i];
            arr[i] = t;
        }
}

static void nextstate(struct jumbler *j)
{
    bool alpha = isalpha(j->lastchar);
    switch (j->state)
        {
        case STATE_START:
            j->state = alpha ? STATE_ALPHA_FIRST : STATE_NONALPHA;
            return;
        case STATE_ALPHA_FIRST:
            j->state = alpha ? STATE_ALPHA_N : STATE_NONALPHA;
            return;
        case STATE_ALPHA_N:
            j->state = alpha ? STATE_ALPHA_N : STATE_JUMBLE;
            return;
        case STATE_NONALPHA:
            j->state = alpha ? STATE_ALPHA_FIRST : STATE_NONALPHA;
            return;
        case STATE_JUMBLE:
            j->state = alpha ? STATE_ALPHA_FIRST : STATE_NONALPHA;
            return;
        }
}

static char jumbler_next(struct txtmac *tm)
{
    struct jumbler *priv = (struct jumbler *)tm;
    return '0'; // TODO
}

struct txtmac *minit_jumbler(struct txtmac *tm)
{
    struct jumbler *priv = malloc(sizeof(struct jumbler));
    if (priv == NULL)
        {
            return NULL;
        }

    priv->src = tm;
    priv->bufidx = 0;
    priv->lastchar = '0'; // TODO: what is a good start?

    priv->tm.priv = priv;
    priv->tm.next = jumbler_next;

    return &priv->tm;
}
