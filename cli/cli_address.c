#include "cli.h"

/**
 * cli_address - print the public key (address) of the current wallet
 *
 * other people use this address to send coins to this wallet
 *
 * @state: CLI state
 *
 * Return: EXIT_SUCCESS, or 2 if there are too many arguments
 */
int cli_address(state_t *state)
{
	uint8_t pub[EC_PUB_LEN];
	int i;

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
	for (i = 0; i < EC_PUB_LEN; i++)
		printf("%02x", pub[i]);
	printf("\n");
	return ((state->status = EXIT_SUCCESS));
}
