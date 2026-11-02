#include "../blockchain.h"

/**
 * _block_print - print one block
 * @node: block_t *
 * @idx: index in the chain (unused)
 * @arg: unused
 * Return: 0
 */
static int _block_print(llist_node_t node, unsigned int idx, void *arg)
{
	block_t const *block = node;

	(void)idx, (void)arg;
	printf("\t\tBlock: {\n\t\t\tinfo: {\n");
	printf("\t\t\t\tindex: %u,\n", block->info.index);
	printf("\t\t\t\tdifficulty: %u,\n", block->info.difficulty);
	printf("\t\t\t\ttimestamp: %lu,\n", (unsigned long)block->info.timestamp);
	printf("\t\t\t\tnonce: %lu,\n", (unsigned long)block->info.nonce);
	printf("\t\t\t\tprev_hash: ");
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf("\n\t\t\t},\n\t\t\tdata: {\n\t\t\t\tbuffer: \"%.*s\",\n",
	       (int)block->data.len, (char const *)block->data.buffer);
	printf("\t\t\t\tlen: %u\n\t\t\t},\n\t\t\thash: ", block->data.len);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n\t\t}\n");
	return (0);
}

/**
 * _blockchain_print - print a whole blockchain
 * @blockchain: blockchain to print
 */
void _blockchain_print(blockchain_t const *blockchain)
{
	if (!blockchain)
	{
		printf("Blockchain: NULL\n");
		return;
	}
	printf("Blockchain: {\n\tchain [%d]: [\n", llist_size(blockchain->chain));
	llist_for_each(blockchain->chain, _block_print, NULL);
	printf("\t]\n}\n");
}
