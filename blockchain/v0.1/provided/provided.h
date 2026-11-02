#ifndef _PROVIDED_H_
#define _PROVIDED_H_

#include <stdint.h>
#include <stddef.h>
#include "endianness.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

struct blockchain_s;
void _blockchain_print(struct blockchain_s const *blockchain);

#endif /* _PROVIDED_H_ */
