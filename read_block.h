#ifndef READ_BLOCK_H
#define READ_BLOCK_H
#include <stdio.h>

/**
 * @brief Read next block of text from stream.
 *
 * @param _s Pointer to a valid `char *`, if the `char *` is NULL it will
 * allocate a new string and store the capacity in `_capp`, if the `char *`
 * is a valid buffer and `_capp` is larger than `0` it will use the buffer
 * and realloc if needed.
 * @param _capp Pointer to a `size_t`, look at the `_s` parameter for
 * more info.
 * @param _stream Stream to read from.
 * @return Amount of bytes read from stream or `(size_t)-1` for EOF and errors.
 */
size_t read_block(char **restrict _s, size_t *restrict _capp, FILE *restrict _stream);

#endif /* READ_BLOCK_H */
