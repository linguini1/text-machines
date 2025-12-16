#include <stdlib.h>

#include "api.h"

void mdestroy(struct txtmac *tm)
{
    if (tm == NULL) return;

    /* Default destruction method */

    if (tm->destroy == NULL)
        {
            return free(tm);
        }

    return tm->destroy(tm);
}
