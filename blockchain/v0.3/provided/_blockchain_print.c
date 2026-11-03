#include "blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);
void _transaction_print(transaction_t const *transaction);
int _transaction_print_brief_loop(transaction_t const *transaction,
	unsigned int idx, char const *indent);

/**
 * _block_print - print a Block
 *
 * @block: Block to print
 * @idx: unused
 * @indent: indentation
 *
 * Return: 0
 */
static int _block_print(block_t const *block, unsigned int idx,
	char const *indent)
{
	int i;

	printf("%sBlock: {\n", indent);
	printf("%s\tinfo: {\n", indent);
	printf("%s\t\tindex: %u,\n", indent, block->info.index);
	printf("%s\t\tdifficulty: %u,\n", indent, block->info.difficulty);
	printf("%s\t\ttimestamp: %lu,\n", indent,
		(unsigned long)block->info.timestamp);
	printf("%s\t\tnonce: %lu,\n", indent, (unsigned long)block->info.nonce);
	printf("%s\t\tprev_hash: ", indent);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf("\n%s\t},\n", indent);
	printf("%s\tdata: {\n", indent);
	printf("%s\t\tbuffer: \"%.*s\",\n", indent, (int)block->data.len,
		(char const *)block->data.buffer);
	printf("%s\t\tlen: %u\n%s\t},\n", indent, block->data.len, indent);
	printf("%s\ttransactions [%d]: [\n", indent,
		llist_size(block->transactions));
	for (i = 0; i < llist_size(block->transactions); i++)
		_transaction_print(llist_get_node_at(block->transactions, i));
	printf("%s\t]\n", indent);
	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n%s}\n", indent);
	(void)idx;
	return (0);
}

/**
 * _block_print_brief - print a Block on a few lines
 *
 * @block: Block to print
 * @idx: unused
 * @indent: indentation
 *
 * Return: 0
 */
static int _block_print_brief(block_t const *block, unsigned int idx,
	char const *indent)
{
	char indent2[32];

	printf("%sBlock: {\n", indent);
	printf("%s\tinfo: { %u, %u, %lu, %lu, ", indent, block->info.index,
		block->info.difficulty, (unsigned long)block->info.timestamp,
		(unsigned long)block->info.nonce);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf(" },\n");
	printf("%s\tdata: { \"%.*s\", %u },\n", indent, (int)block->data.len,
		(char const *)block->data.buffer, block->data.len);
	printf("%s\ttransactions [%d]: [\n", indent,
		llist_size(block->transactions));
	snprintf(indent2, sizeof(indent2), "%s\t\t", indent);
	llist_for_each(block->transactions,
		(node_func_t)_transaction_print_brief_loop, indent2);
	printf("%s\t]\n", indent);
	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n%s}\n", indent);
	(void)idx;
	return (0);
}

/**
 * _blockchain_print - print a Blockchain
 *
 * @blockchain: Blockchain to print
 */
void _blockchain_print(blockchain_t const *blockchain)
{
	if (!blockchain)
		return;
	printf("Blockchain: {\n");
	printf("\tchain [%d]: [\n", llist_size(blockchain->chain));
	llist_for_each(blockchain->chain, (node_func_t)_block_print, "\t\t");
	printf("\t]\n}\n");
}

/**
 * _blockchain_print_brief - print a Blockchain on a few lines
 *
 * @blockchain: Blockchain to print
 */
void _blockchain_print_brief(blockchain_t const *blockchain)
{
	if (!blockchain)
		return;
	printf("Blockchain: {\n");
	printf("\tchain [%d]: [\n", llist_size(blockchain->chain));
	llist_for_each(blockchain->chain, (node_func_t)_block_print_brief,
		"\t\t");
	printf("\t]\n}\n");
}
