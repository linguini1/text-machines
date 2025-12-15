#include <stdlib.h>

#include "api.h"

void mdestroy(struct txtmac *tm) { free(tm->priv); }
