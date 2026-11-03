#include "cli.h"

/**
 * cli_balance - print how many coins the current wallet owns
 *
 * the balance is the sum of the unspent outputs sent to this wallet
 *
 * @state: CLI state
 *
 * Return: EXIT_SUCCESS, or 2 if there are too many arguments
 */
int cli_balance(state_t *state)
{
	uint8_t pub[EC_PUB_LEN];
	unspent_tx_out_t *unspent;
	unsigned long balance = 0;
	int i, count = 0;

	if (state->argc > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", state->argv[0]);
		return ((state->status = 2));
	}
	if (!ec_to_pub(state->wallet, pub))
	{
		fprintf(stderr, "%s: no wallet loaded\n", state->argv[0]);
		return ((state->status = EXIT_FAILURE));
	}
	for (i = 0; i < llist_size(state->blockchain->unspent); i++)
	{
		unspent = llist_get_node_at(state->blockchain->unspent, i);
		if (!memcmp(unspent->out.pub, pub, EC_PUB_LEN))
		{
			balance += unspent->out.amount;
			count++;
		}
	}
	printf("Balance: %lu coins (in %d unspent outputs)\n", balance, count);
	return ((state->status = EXIT_SUCCESS));
}
