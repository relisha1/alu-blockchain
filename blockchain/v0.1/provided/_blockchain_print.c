#include "../blockchain.h"

/**
 * _print_hex_buffer - print a buffer in hexadecimal
 *
 * @buf: buffer
 * @len: number of bytes
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; buf && i < len; i++)
		printf("%02x", buf[i]);
}

/**
 * _block_print - print one Block
 *
 * @block: Block to print
 * @idx: index of the Block in the chain (unused)
 * @indent: string printed at the start of each line
 *
 * Return: 0
 */
static int _block_print(block_t const *block, unsigned int idx,
	char const *indent)
{
	(void)idx;
	if (!block)
	{
		printf("%snil\n", indent);
		return (0);
	}
	printf("%sBlock: {\n%s\tinfo: {\n", indent, indent);
	printf("%s\t\tindex: %u,\n", indent, block->info.index);
	printf("%s\t\tdifficulty: %u,\n", indent, block->info.difficulty);
	printf("%s\t\ttimestamp: %lu,\n", indent,
		(unsigned long)block->info.timestamp);
	printf("%s\t\tnonce: %lu,\n", indent, (unsigned long)block->info.nonce);
	printf("%s\t\tprev_hash: ", indent);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf("\n%s\t},\n%s\tdata: {\n", indent, indent);
	printf("%s\t\tbuffer: \"%.*s\",\n", indent, (int)block->data.len,
		(char const *)block->data.buffer);
	printf("%s\t\tlen: %u\n%s\t},\n", indent, block->data.len, indent);
	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n%s}\n", indent);
	return (0);
}

/**
 * _block_print_brief - print one Block on a few lines
 *
 * @block: Block to print
 * @idx: index of the Block in the chain (unused)
 * @indent: string printed at the start of each line
 *
 * Return: 0
 */
static int _block_print_brief(block_t const *block, unsigned int idx,
	char const *indent)
{
	(void)idx;
	if (!block)
	{
		printf("%snil\n", indent);
		return (0);
	}
	printf("%sBlock: {\n", indent);
	printf("%s\tinfo: { %u, %u, %lu, %lu, ", indent, block->info.index,
		block->info.difficulty, (unsigned long)block->info.timestamp,
		(unsigned long)block->info.nonce);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf(" },\n%s\tdata: { \"%.*s\", %u },\n", indent,
		(int)block->data.len, (char const *)block->data.buffer,
		block->data.len);
	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n%s}\n", indent);
	return (0);
}

/**
 * _blockchain_print - print a Blockchain in full
 *
 * @blockchain: Blockchain to print
 */
void _blockchain_print(blockchain_t const *blockchain)
{
	int i, size;

	if (!blockchain)
	{
		printf("Blockchain: nil\n");
		return;
	}
	size = llist_size(blockchain->chain);
	printf("Blockchain: {\n\tchain [%d]: [\n", size);
	for (i = 0; i < size; i++)
		_block_print(llist_get_node_at(blockchain->chain, i), i, "\t\t");
	printf("\t]\n}\n");
}

/**
 * _blockchain_print_brief - print a Blockchain in brief form
 *
 * @blockchain: Blockchain to print
 */
void _blockchain_print_brief(blockchain_t const *blockchain)
{
	int i, size;

	if (!blockchain)
	{
		printf("Blockchain: nil\n");
		return;
	}
	size = llist_size(blockchain->chain);
	printf("Blockchain: {\n\tchain [%d]: [\n", size);
	for (i = 0; i < size; i++)
		_block_print_brief(llist_get_node_at(blockchain->chain, i), i, "\t\t");
	printf("\t]\n}\n");
}
