#include "../blockchain.h"

/**
 * _genesis - get the Genesis Block, first block of every Blockchain
 *
 * Return: pointer to a static, read-only copy of the Genesis Block
 */
block_t const *_genesis(void)
{
	static block_t const genesis = GENESIS_BLOCK;

	return (&genesis);
}
