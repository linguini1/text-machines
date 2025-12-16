# Text Machines

This document describes the behaviour of the implemented text machines.

In general, all text machines have a private state and a `next()` function which returns their next character of output.
Once a text machine has no more output to give, it will return an `EOF` character.

All text machines returned my `minit_*` functions are `malloc`'d and must be freed using the provided `mdestroy()`
function.

## Generative Machines

These text machines generate an actual output which can be consumed by a [filtering machine](#filtering-machines).

### Buffer

This machine generates its output from a given buffer. It can be used as follows:

```c
char mytext[] = "This is some text!\n";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
```

This allows other text machines to iterate over the buffer. Once the buffer has been depleted, the machine will output
`EOF` to signal the end of its contents.

The passed buffer must not be `NULL`, or the returned machine will be `NULL`.

### File

This machine generates its output from a given file, using the C library `FILE` i/o structure. It can be used as
follows:

```c
FILE *myfile = fopen("path/to/myfile.txt", "r");
if (myfile == NULL) {
    /* Error handling */
}
struct txtmac *stream = minit_file(myfile);
```

This will allow other machines to iterate over the file character by character. When the end of the file is reached,
this machine will output `EOF` to signal the end of its contents.

If the passed `file` is `NULL`, the returned text machine will be null.

### File Descriptor (`fd`)

This machine generates its output from the device/file behind the given Unix file descriptor. It can be used as follows:

```c
int *myfd = open("path/to/myfile.txt", O_RDONLY);
if (myfd < 0) {
    /* Error handling */
}
struct txtmac *stream = minit_fd(myfd);
```

This will allow other machines to iterate over the file character by character. When the end of the file is reached,
this machine will output `EOF` to signal the end of its contents.

If the passed file descriptor is negative, the returned text machine will be `NULL`.

### Letter

This machine generates output composed of a single letter. It is up to the caller to choose which letter and how many of
it should be output. It can be used as follows:

```c
struct txtmac *stream = minit_letter('a', 5);
```

The output resulting from this machine will be:
```
aaaaa
```
followed by the `EOF` character signalling the end of the stream.

If the value passed as `count` is 0, the machine will never output an `EOF` character and will simply output the chosen
letter infinitely.

## Filtering Machines

These machines act on an output from a [generative machine](#generative-machines). They are characterized by one of
their initialization arguments being another text machine, whose output will be modified according to the machine's
behaviour.

Below is a list of the implemented filtering text machines and some example uses.

### Applicator

This text machine applies a mapping function to every character in its stream. It can be used as follows:

```c
char tolower_c(char c) {
    return tolower(c);
}

char mytext[] = "ThiS is sOme tExt!";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
struct txtmac *tm = minit_applicator(stream, tolower_c);
```

The output of the machine `tm` would be:
```
ThiS is sOme tExt!
```
followed by the `EOF` character.

The returned text machine will be `NULL` if the provided stream is `NULL` or if the provided function pointer to `op` is
`NULL`.

### Capitalize

This text machine capitalizes text according to punctuation rules (capital letters follow punctuation). It can be used
as follows:

```c
char mytext[] = "this is some text! and some more!";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
struct txtmac *tm = minit_capitalize(stream);
```

The output of the machine `tm` would be:
```
This is some text! And some more!
```
followed by the `EOF` character.

The returned text machine will be `NULL` if the provided stream is `NULL`.

### Jumbler

This text machine is based on the implementation of the [jumbler][jumbler] project, which programmatically creates the
[transposed word effect](https://en.wikipedia.org/wiki/Transposed_letter_effect). In essence, this machine will shuffle
every word it encounters such that the first and last letters remain in place, but the letters between are randomly
shuffled. A word is considered to be a sequence of alphabetical characters, separated on either end by a non-alphabetic
character (punctuation, white space, number, etc.). It can be used as follows:

```c
char mytext[] = "Test text containing some arguably long words to demonstrate the jumbling effect.";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
struct txtmac *tm = minit_jumbler(stream);
```

The output of the machine `tm` _might_ be:
```
Tset txet cntiinanog some alrbguay long wodrs to dnomttaerse the jlumnibg eecfft.
```
followed by the `EOF` character. The term "_might_" is used, as the letters are randomly shuffled and thus may produce a
different output for each execution.

The input stream must not be `NULL`, or the returned text machine will be `NULL`.

### Replace

This text machine replaces a search term with some other text. It can be used as follows:

```c
char mytext[] = "This is some text!";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
struct txtmac *tm = minit_replace(stream, "text", "bullshit");
```

The output of the machine `tm` would be:
```
This is some bullshit!
```
followed by the `EOF` character.

The replacement text can be provided as either `NULL` or the empty string `""` to simply remove the search term from the
text. The modified example

```c
char mytext[] = "This is some text!";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
struct txtmac *tm = minit_replace(stream, "text", NULL);
```
yields the output
```
This is some !
```
followed by the `EOF` character.

All occurrences where the search term is matched will be replaced. This may produce counter-intuitive results:

```c
char mytext[] = "This is some text!";
struct txtmac *stream = minit_buf(mytext, sizeof(mytext));
struct txtmac *tm = minit_replace(stream, "is", "was");
```
yields the output
```
Thwas was some text!
```
followed by the `EOF` character.

If the `old` parameter (search term) is `NULL` or the stream is `NULL`, the returned text machine will be `NULL`. If the
`old` parameter is the empty string `""`, the text will not be modified regardless of the `new` (replacement text)
parameter's value.

[jumbler]: https://github.com/linguini1/jumbler
