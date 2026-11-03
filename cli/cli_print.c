#include "cli.h"

/**
 * print_hash - print a hash in hexadecimal
 *
 * @hash: hash to print
 */
static void print_hash(uint8_t const hash[SHA256_DIGEST_LENGTH])
{
	int i;

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", hash[i]);
}

/**
 * cli_print - print a short view of every Block in the Blockchain
 *
 * @state: CLI state
 *
 * Return: EXIT_SUCCESS, or 2 if there are too many arguments
 */
int cli_print(state_t *state)
{
	block_t *block;
	int i, nb_tx;

	if (state->argc > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", state->argv[0]);
		return ((state->status = 2));
	}
	for (i = 0; i < llist_size(state->blockchain->chain); i++)
	{
		block = llist_get_node_at(state->blockchain->chain, i);
		nb_tx = block->transactions ? llist_size(block->transactions) : 0;
		printf("Block %u | difficulty %u | %d transaction(s)\n",
			block->info.index, block->info.difficulty, nb_tx);
		printf("  hash: ");
		print_hash(block->hash);
		printf("\n");
		if (i < llist_size(state->blockchain->chain) - 1)
			printf("    |\n");
	}
	return ((state->status = EXIT_SUCCESS));
}
