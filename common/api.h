#ifndef _COMMON_API_H_
#define _COMMON_API_H_

#include <stdio.h>

/* Machine APIs */

struct txtmac
{
    /* This function returns the next character to be output by the text
     * machine.
     *
     * - self: A reference to this text machine. Cannot be NULL.
     *
     * Returns: The next character to be output by this text machine.
     */
    char (*next)(struct txtmac *self);

    /* This function performs the tear-down/destruction logic for the text
     * machine.
     *
     * - self: A reference to this text machine. Cannot be NULL.
     */
    void (*destroy)(struct txtmac *self);

    /* This member contains private lower-half information from the text machine
     * implementation. It must be allocated using `malloc` and safe to call
     * `free` on without special tear-down logic.
     */
    void *priv;
};

/* Destroys a text machine by releasing its memory.
 *
 * - tm: The text machine to destroy
 */
void mdestroy(struct txtmac *tm);

/* Sink options for machine output */

/* Sinks the output of a text machine to a file pointed to by a file descriptor.
 *
 * - tm: The text machine to collect the output of
 * - fd: The file descriptor of the file where the output of `tm` should be
 *   stored.
 *
 * Returns: 0 on success, error code on failure.
 */
int sink_fd(struct txtmac *tm, int fd);

/* Sinks the output of a text machine to a file pointed to by a file pointer.
 *
 * - tm: The text machine to collect the output of
 * - fd: The file pointer of the file where the output of `tm` should be
 *   stored.
 *
 * Returns: 0 on success, error code on failure.
 */
int sink_file(struct txtmac *tm, FILE *file);

/* Sinks the output of a text machine to a buffer. `EOF` is replaced by the null
 * terminator '\0'. The buffer size will be respected; this function will only
 * fill the buffer with up to `n` characters from the text machine.
 *
 * - tm: The text machine to collect the output of. Cannot be NULL.
 * - buf: The buffer where the output of `tm` should be stored. Cannot be NULL.
 * - n: The size of the buffer in characters.
 *
 * Returns: The number of bytes written to `buf`. 0 bytes on error.
 */
size_t sink_buf(struct txtmac *tm, char *buf, size_t n);

#endif // _COMMON_API_H_
