#ifndef _MACHINES_H_
#define _MACHINES_H_

#include <stdio.h>
#include <stdlib.h>

#include "api.h"

/* Generative machines */

struct txtmac *minit_file(FILE *src);
struct txtmac *minit_fd(int fd);
struct txtmac *minit_buf(const char *buf, size_t n);
struct txtmac *minit_letter(char letter, size_t count);

/* Filtering machines */

typedef char(charop)(char);

struct txtmac *minit_applicator(struct txtmac *tm, charop *op);
struct txtmac *minit_jumbler(struct txtmac *tm);
struct txtmac *minit_replace(struct txtmac *tm, const char *old, const char *new);

#endif // _MACHINES_H_
