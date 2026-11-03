#include "cli.h"

/**
 * same_output - check if an unspent output is the one an input refers to
 *
 * @node: unspent output
 * @arg: transaction input
 *
 * Return: 1 if it matches, 0 otherwise
 */
static int same_output(llist_node_t node, void *arg)
{
	unspent_tx_out_t *unspent = node;
	tx_in_t *in = arg;

	return (!memcmp(unspent->out.hash, in->tx_out_hash,
		SHA256_DIGEST_LENGTH));
}

/**
 * remove_spent - remove from a list the outputs spent by a transaction
 *
 * @unspent: list of unspent outputs (nodes are not freed)
 * @transaction: transaction that spends them
 */
static void remove_spent(llist_t *unspent, transaction_t *transaction)
{
	int i;

	for (i = 0; i < llist_size(transaction->inputs); i++)
		llist_remove_node(unspent, same_output,
			llist_get_node_at(transaction->inputs, i), 0, NULL);
}

/**
 * pick_transactions - move valid transactions from the pool to a Block
 *
 * Transactions are checked one by one. Each accepted transaction removes
 * the outputs it spends, so the same coins cannot be spent twice.
 * Invalid transactions are deleted.
 *
 * @state: CLI state
 * @block: Block to fill
 *
 * Return: 0 on success, -1 on failure
 */
static int pick_transactions(state_t *state, block_t *block)
{
	llist_t *working = llist_create(MT_SUPPORT_FALSE);
	transaction_t *transaction;
	int i;

	if (!working)
		return (-1);
	for (i = 0; i < llist_size(state->blockchain->unspent); i++)
		llist_add_node(working,
			llist_get_node_at(state->blockchain->unspent, i),
			ADD_NODE_REAR);

	transaction = llist_pop(state->tx_pool);
	while (transaction)
	{
		if (transaction_is_valid(transaction, working))
		{
			llist_add_node(block->transactions, transaction,
				ADD_NODE_REAR);
			remove_spent(working, transaction);
		}
		else
			transaction_destroy(transaction);
		transaction = llist_pop(state->tx_pool);
	}
	llist_destroy(working, 0, NULL);
	return (0);
}

/**
 * cli_mine_process - fill, mine and add a Block to the Blockchain
 *
 * @state: CLI state
 * @block: Block to mine
 * @prev_block: previous Block in the chain
 * @coinbase_tx: coinbase transaction of the new Block
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
static int cli_mine_process(state_t *state, block_t *block,
			    block_t *prev_block, transaction_t *coinbase_tx)
{
	llist_t *unspent;

	llist_add_node(block->transactions, coinbase_tx, ADD_NODE_FRONT);
	if (pick_transactions(state, block) == -1)
	{
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	block->info.difficulty = blockchain_difficulty(state->blockchain);
	block_mine(block);

	if (block_is_valid(block, prev_block, state->blockchain->unspent) != 0)
	{
		fprintf(stderr, "%s: failed to mine a valid block\n",
			state->argv[0]);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}

	unspent = update_unspent(block->transactions, block->hash,
		state->blockchain->unspent);
	if (!unspent)
	{
		fprintf(stderr, "%s: failed to update unspent outputs\n",
			state->argv[0]);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	state->blockchain->unspent = unspent;
	llist_add_node(state->blockchain->chain, block, ADD_NODE_REAR);
	fprintf(stdout, "Successfully mined a block\n");

	return ((state->status = EXIT_SUCCESS));
}



/**
 * cli_mine - program that initiates the mining of a block,
 * handling all transactions and updates
 *
 * @state: the current state of the blockchain
 *
 * Return: the status code indicating success or failure of the mining process
 */

int cli_mine(state_t *state)
{
	int8_t block_data[BLOCKCHAIN_DATA_MAX] = {0};
	block_t *block = NULL;
	block_t *prev_block = llist_get_tail(state->blockchain->chain);
	transaction_t *coinbase_tx = NULL;

	if (state->argc > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", state->argv[0]);
		return ((state->status = 2));
	}

	block = block_create(prev_block, block_data, BLOCKCHAIN_DATA_MAX);

	if (!block)
	{
		fprintf(stderr, "%s: failed to create block\n", state->argv[0]);
		return ((state->status = EXIT_FAILURE));
	}

	coinbase_tx = coinbase_create(state->wallet, block->info.index);

	if (!coinbase_tx || !coinbase_is_valid(coinbase_tx, block->info.index))
	{
		fprintf(stderr, "%s: failed to create coinbase transaction\n",
			state->argv[0]);
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}

	return (cli_mine_process(state, block, prev_block, coinbase_tx));
}

