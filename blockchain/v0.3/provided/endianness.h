#ifndef _ENDIANNESS_H_
#define _ENDIANNESS_H_

#include <stdint.h>
#include <stddef.h>

#define SWAPENDIAN(x) _swap_endian((void *)&(x), sizeof(x))

uint8_t _get_endianness(void);
void _swap_endian(void *p, size_t size);

#endif /* _ENDIANNESS_H_ */
