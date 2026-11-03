#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/**
 * _print_hex_buffer - print a buffer in hexadecimal
 *
 * @buf: buffer to print
 * @len: number of bytes to print
 */
void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; buf && i < len; i++)
		printf("%02x", buf[i]);
}
