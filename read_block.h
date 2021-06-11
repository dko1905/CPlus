#ifndef READ_BLOCK_H
#define READ_BLOCK_H
#include <stdio.h>
#include <wchar.h>

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
/**
 * @brief Read next block of text from `_stream` to wide char string.
 *
 * @param _wcs Pointer to a valid `wchar_t *`, if the `wchar_t *` is NULL it will
 * allocate a new string and store the capacity in `_capp`, if the `char *`
 * is a valid buffer and `_capp` is larger than `0` it will use the buffer
 * and realloc if needed.
 * @param _capp Pointer to capacity. Be careful, the capacity and len are not
 * the same, because we are using wide strings.
 * @param _stream Input stream.
 * @return Charectores read or `(size_t)-1` for EOF and errors.
 */
size_t read_block_wcs(wchar_t **restrict _wcs, size_t *restrict _capp, FILE *restrict _stream);

#endif /* READ_BLOCK_H */
