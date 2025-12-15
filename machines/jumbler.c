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
    size_t count;
    bool output;
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

static void jumble_nextstate(struct jumbler *j)
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
    if (tm == NULL) return EOF;

    for (;;)
        {
            /* If we can output a buffered character, output one */

            if (priv->output && priv->bufidx < priv->count)
                {
                    return priv->buf[priv->bufidx++];
                }
            else if (priv->output)
                {
                    /* All characters output. Return the final non-alpha char
                     * which triggered the end of word detection.
                     */

                    priv->count = 0;
                    priv->output = false;
                    return priv->lastchar;
                }

            /* Get the next character to work with */

            priv->lastchar = priv->src->next(priv->src);
            jumble_nextstate(priv);

            /* Execute state logic, outputting characters if we can */

            switch (priv->state)
                {
                case STATE_START:
                    /* Purposeful nothing to do */
                    break;
                case STATE_ALPHA_N:
                    priv->buf[priv->count++] = priv->lastchar;
                    break;
                case STATE_ALPHA_FIRST:
                case STATE_NONALPHA:
                    return priv->lastchar;
                case STATE_JUMBLE:
                    shuffle(priv->buf, priv->count - 1);
                    priv->output = true;
                    priv->bufidx = 0;
                    break;
                }
        }

    return EOF;
}

struct txtmac *minit_jumbler(struct txtmac *tm)
{
    if (tm == NULL) return NULL;

    struct jumbler *priv = malloc(sizeof(struct jumbler));
    if (priv == NULL) return NULL;

    priv->src = tm;
    priv->bufidx = 0;
    priv->count = 0;
    priv->output = false;
    priv->lastchar = '\0';

    priv->tm.priv = priv;
    priv->tm.next = jumbler_next;

    return &priv->tm;
}
