#include "blockchain.h"

/**
* hash_matches_difficulty - checks if hash matches difficulty.
* @hash: hash to check.
* @difficulty: difficulty of a block.
* Return: 1 if hash matches difficulty, 0 otherwise.
* Author: Frank Onyema Orji.
*/
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],

uint32_t difficulty)
{
	uint32_t i, mod, byte;

	if (!hash || difficulty > SHA256_DIGEST_LENGTH * 8)
		return (0);
	mod = difficulty / 8;
	byte = difficulty % 8;
	for (i = 0; i < mod; i++)
	{
		if (hash[i] != 0)
			return (0);
	}
	if (byte && (hash[mod] >> (8 - byte)))
		return (0);
	return (1);
}

