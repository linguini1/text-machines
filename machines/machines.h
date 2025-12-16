#ifndef _MACHINES_H_
#define _MACHINES_H_

#include <stdio.h>
#include <stdlib.h>

#include "api.h"

/* Generative machines */

/* Creates and initializes a "file" text machine whose output will be drawn from
 * `src`.
 *
 * - src: A non-null FILE pointer where the machine output will be obtained
 *   from.
 *
 * Returns: A pointer to a "file" text machine allocated on the heap, or NULL if
 * there was an error.
 */
struct txtmac *minit_file(FILE *src);

/* Creates and initializes a "file descriptor" text machine whose output will be
 * drawn from the file associated with `fd`.
 *
 * - fd: A non-negative file descriptor where the machine output will be
 *   obtained from.
 *
 * Returns: A pointer to a "file descriptor" text machine allocated on the heap,
 * or NULL if there was an error.
 */
struct txtmac *minit_fd(int fd);

/* Creates and initializes a "buffer" text machine whose output will be
 * drawn from the provided buffer.
 *
 * - buf: A non-null character buffer
 * - n: The number of characters in `buf`
 *
 * Returns: A pointer to a "buffer" text machine allocated on the heap,
 * or NULL if there was an error.
 */
struct txtmac *minit_buf(const char *buf, size_t n);

/* Creates and initializes a "letter" text machine whose output will be a
 * sequence of a single letter.
 *
 * - letter: The letter to output
 * - count: The number of times to output `letter` before `EOF`. If 0, `letter`
 *   will be output infinitely many times.
 *
 * Returns: A pointer to a "letter" text machine allocated on the heap,
 * or NULL if there was an error.
 */
struct txtmac *minit_letter(char letter, size_t count);

/* Filtering machines */

typedef char(charop)(char);

/* Creates and initializes an "applicator" text machine.
 *
 * - tm: The text machine whose output is to be modified by the applicator.
 *   Cannot be NULL.
 * - op: The operation applied to each character output by `tm`. Cannot be NULL.
 *
 * Returns: A pointer to an "applicator" text machine allocated on the heap,
 * or NULL if there was an error.
 */
struct txtmac *minit_applicator(struct txtmac *tm, charop *op);

/* Creates and initializes a "jumbler" text machine.
 *
 * - tm: The text machine whose output is to be jumbled. Cannot be NULL.
 *
 * Returns: A pointer to a "jumbler" text machine allocated on the heap,
 * or NULL if there was an error.
 */
struct txtmac *minit_jumbler(struct txtmac *tm);

/* Creates and initializes a "replace" text machine.
 *
 * - tm: The text machine whose output is to be modified by the "replace" text
 *   machine. Cannot be NULL.
 * - old: The term to have all instances replaced by `new`. Can be the empty
 *   string to have no effect. Cannot be NULL.
 * - new: The new term to replace all instance of `old`. Can be NULL or empty
 *   string to have the behaviour of just removing `old`.
 *
 * Returns: A pointer to a "replace" text machine allocated on the heap, or NULL
 * if there was an error.
 */
struct txtmac *minit_replace(struct txtmac *tm, const char *old,
                             const char *new);

/* Creates and initializes a "capitalize" text machine.
 *
 * - tm: The text machine whose output is to be modified by the "capitalize"
 *   text machine. Cannot be NULL.
 *
 * Returns: A pointer to a "capitalize" text machine allocated on the heap, or
 * NULL if there was an error.
 */
struct txtmac *minit_capitalize(struct txtmac *tm);

/* Creates and initializes a "caesar" text machine.
 *
 * - tm: The text machine whose output is to be modified by the "caesar"
 *   text machine. Cannot be NULL.
 * - shift: The shift (left) to use for the Caesar cipher. Must be between 0
 *   and 25.
 *
 * Returns: A pointer to a "caesar" text machine allocated on the heap, or
 * NULL if there was an error.
 */
struct txtmac *minit_caesar(struct txtmac *tm, unsigned shift);

#endif // _MACHINES_H_
