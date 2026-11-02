#include "../blockchain.h"

/**
 * _blockchain_destroy - delete a Blockchain and all the Blocks it contains
 *
 * @blockchain: Blockchain to delete
 */
void _blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;
	llist_destroy(blockchain->chain, 1, NULL);
	free(blockchain);
}
