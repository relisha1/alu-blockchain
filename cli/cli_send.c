#include "cli.h"

/**
 * parse_address - convert a public key written in hexadecimal into bytes
 *
 * @hex: public key as 130 hexadecimal characters
 * @pub: buffer to store the 65 bytes of the key
 *
 * Return: 0 on success, -1 if @hex is not a valid public key
 */
static int parse_address(char const *hex, uint8_t pub[EC_PUB_LEN])
{
	unsigned int byte;
	size_t i;

	if (strlen(hex) != EC_PUB_LEN * 2)
		return (-1);
	for (i = 0; i < EC_PUB_LEN; i++)
	{
		if (sscanf(hex + (2 * i), "%2x", &byte) != 1)
			return (-1);
		pub[i] = (uint8_t)byte;
	}
	return (0);
}



/**
 * handle_transaction_send - program that attempts to create
 * and send a transaction based on provided receiver's public key,
 * amount to transfer, and state information
 *
 * @state: the current state containing wallet, blockchain,
 *         and transaction pool information
 * @amount: the amount of cryptocurrency to send
 * @pub: the public key of the receiver, in bytes,
 *       used to identify the recipient
 *
 * Return: - EXIT_SUCCESS if the transaction is successfully
 *           added to the local transaction pool,
 *         - otherwise, returns EXIT_FAILURE
 */

static int handle_transaction_send(state_t *state, uint32_t amount,
				   uint8_t pub[EC_PUB_LEN])
{
	EC_KEY *receiver = NULL;
	transaction_t *tx = NULL;

	receiver = ec_from_pub(pub);
	if (!receiver)
	{
		fprintf(stderr, "%s: %s: invalid receiver public key\n",
			state->argv[0], state->argv[2]);
		return ((state->status = EXIT_FAILURE));
	}
	tx = transaction_create(state->wallet, receiver, amount,
		state->blockchain->unspent);
	EC_KEY_free(receiver);
	if (!tx)
	{
		fprintf(stderr,
			"%s: failed to create transaction (not enough coins?)\n",
			state->argv[0]);
		return ((state->status = EXIT_FAILURE));
	}
	if (!transaction_is_valid(tx, state->blockchain->unspent) ||
		llist_add_node(state->tx_pool, tx, ADD_NODE_REAR) == -1)
	{
		fprintf(stderr, "%s: invalid transaction\n", state->argv[0]);
		transaction_destroy(tx);
		return ((state->status = EXIT_FAILURE));
	}
	fprintf(stdout, "Transaction added to local transaction pool\n");
	return ((state->status = EXIT_SUCCESS));
}



/**
 * cli_send - program that processes the input parameters
 * to extract a public key and a transfer amount, then calls
 * 'handle_transaction_send' to attempt the transaction
 *
 * @state: the current CLI state including arguments from the command line
 *
 * Return: 2 on argument errors, EXIT_FAILURE on processing errors,
 *         or the result of 'handle_transaction_send'
 */

int cli_send(state_t *state)
{
	uint8_t pub[EC_PUB_LEN] = {0};
	unsigned long int amount = 0;
	char *endptr = NULL;

	if (state->argc > 3)
	{
		fprintf(stderr, "%s: too many arguments\n", state->argv[0]);
		return ((state->status = 2));
	}
	if (state->argc < 3)
	{
		fprintf(stderr, "%s: too few arguments\n", state->argv[0]);
		return ((state->status = 2));
	}

	errno = 0;
	amount = strtoul(state->argv[1], &endptr, 0);

	if ((errno != 0 && (amount == 0 || amount == ULONG_MAX)) ||
		(*state->argv[1] == '\0' || *endptr != '\0') ||
		(amount > UINT32_MAX))
	{
		fprintf(stderr, "%s: %s: invalid amount\n",
			state->argv[0], state->argv[1]);

		return ((state->status = EXIT_FAILURE));
	}
	if (parse_address(state->argv[2], pub) == -1)
	{
		fprintf(stderr, "%s: %s: invalid address\n",
			state->argv[0], state->argv[2]);
		return ((state->status = EXIT_FAILURE));
	}

	return (handle_transaction_send(state, amount, pub));
}

