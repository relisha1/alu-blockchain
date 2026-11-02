#include <stdio.h>
#include "provided.h"

/**
 * _print_hex_buffer - print a buffer as hexadecimal
 * @buf: buffer
 * @len: number of bytes
 */
void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; buf && i < len; i++)
		printf("%02x", buf[i]);
}
