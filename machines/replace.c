#include "machines.h"

enum state_e
{
    STATE_NO_MATCH,
    STATE_MATCHING,
    STATE_OUTREPLACEMENT,
    STATE_OUTFAIL,
};

struct replace
{
    struct txtmac tm;
    struct txtmac *src;
    const char *old;
    const char *new;
    size_t oidx;
    size_t nidx;
    enum state_e state;
};

static char replace_next(struct txtmac *tm)
{
    char c;
    struct replace *priv = (struct replace *)tm;
    if (tm == NULL) return EOF;

    for (;;)
        {
            switch (priv->state)
                {
                case STATE_NO_MATCH:
                    /* If the next character starts a match, change states */

                    c = priv->src->next(priv->src);
                    if (c == priv->old[0] && priv->old[0] != '\0')
                        {
                            priv->state = STATE_MATCHING;
                            break;
                        }

                    /* This character wasn't a match, so we can just output it
                     * as-is and continue searching next call.
                     */

                    return c;

                case STATE_MATCHING:
                    priv->oidx++;
                    if (priv->old[priv->oidx] == '\0')
                        {
                            /* If we reached the end of the 'old' string
                             * (complete match) move to outputting.
                             */

                            priv->nidx = 0;
                            priv->state = STATE_OUTREPLACEMENT;
                            break;
                        }

                    /* Check if characters continue to match */

                    c = priv->src->next(priv->src);
                    if (c == priv->old[priv->oidx])
                        {
                            break;
                        }

                    /* If we are here, we failed to match with the character but
                     * we didn't reach the end of the `old` string. This means
                     * we need to output all the characters that we've seen
                     * since the start of STATE_MATCHING until now since the
                     * match failed.
                     */

                    priv->state = STATE_OUTFAIL;
                    priv->nidx = 0; /* We'll use this as the output counter */
                    break;

                case STATE_OUTFAIL:
                    /* Output the old string up to the point where the match
                     * failed.
                     */

                    if (priv->nidx < priv->oidx)
                        {
                            return priv->old[priv->nidx++];
                        }

                    /* We're done the failed output. Finally, output the
                     * character which caused the match to fail.
                     */

                    priv->nidx = 0;
                    priv->oidx = 0;
                    priv->state = STATE_NO_MATCH;
                    return c;

                case STATE_OUTREPLACEMENT:
                    /* Nothing to output; this just removes the old string.
                     */

                    if (priv->new == NULL)
                        {
                            priv->state = STATE_NO_MATCH;
                            priv->oidx = 0;
                            priv->nidx = 0;
                            break;
                        }

                    /* Start outputting new string characters one-by-one */

                    if (priv->new[priv->nidx] != '\0')
                        {
                            return priv->new[priv->nidx++];
                        }

                    /* We hit the null terminator on the new string, go back to
                     * searching.
                     */

                    priv->state = STATE_NO_MATCH;
                    priv->nidx = 0;
                    priv->oidx = 0;
                    break;
                }
        }

    /* Should be unreachable. If we manage to leave the loop, the stream is over
     * because there was an error.
     */

    return EOF;
}

struct txtmac *minit_replace(struct txtmac *tm, const char *old,
                             const char *new)
{
    if (tm == NULL) return NULL;
    if (old == NULL) return NULL;

    struct replace *priv = malloc(sizeof(struct replace));
    if (priv == NULL) return NULL;

    priv->src = tm;
    priv->old = old;
    priv->new = new;
    priv->oidx = 0;
    priv->nidx = 0;
    priv->state = STATE_NO_MATCH;

    priv->tm.priv = priv;
    priv->tm.next = replace_next;
    priv->tm.destroy = NULL;

    return &priv->tm;
}
