#include <stdlib.h>
#include <stdio.h>
#include "blockchain.h"

/**
 * main - Entry point: create a blockchain, print it, free it
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	blockchain_t *blockchain;

	blockchain = blockchain_create();
	if (!blockchain)
	{
		fprintf(stderr, "blockchain_create failed\n");
		return (EXIT_FAILURE);
	}
	_blockchain_print(blockchain);
	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
