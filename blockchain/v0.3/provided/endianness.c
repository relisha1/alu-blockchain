#include "endianness.h"

/**
 * _get_endianness - get the endianness of the running machine
 * Return: 1 for little endian, 2 for big endian
 */
uint8_t _get_endianness(void)
{
	uint16_t x = 1;

	return (*((uint8_t *)&x) ? 1 : 2);
}

/**
 * _swap_endian - reverse the byte order of a value in place
 * @p: address of the value
 * @size: size of the value in bytes
 */
void _swap_endian(void *p, size_t size)
{
	uint8_t *b = p, tmp;
	size_t i;

	for (i = 0; i < size / 2; i++)
	{
		tmp = b[i];
		b[i] = b[size - 1 - i];
		b[size - 1 - i] = tmp;
	}
}
