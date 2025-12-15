#include "machines.h"

struct applicator
{
    struct txtmac tm;
    struct txtmac *src;
    charop *op;
};

static char applicator_next(struct txtmac *tm)
{
    struct applicator *priv = (struct applicator *)(tm);
    return priv->op(priv->src->next(priv->src));
}

struct txtmac *minit_applicator(struct txtmac *tm, charop *op)
{
    struct applicator *priv = malloc(sizeof(struct applicator));
    if (priv == NULL) return NULL;

    priv->op = op;
    priv->src = tm;

    priv->tm.priv = priv;
    priv->tm.next = applicator_next;

    return &priv->tm;
}
