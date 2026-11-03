#include "transaction.h"

/**
 * is_spent - check if an unspent output is used by an input of a transaction
 *
 * @transactions: list of transactions
 * @unspent: unspent output to look for
 *
 * Return: 1 if it is spent, 0 otherwise
 */
static int is_spent(llist_t *transactions, unspent_tx_out_t *unspent)
{
	transaction_t *tx;
	tx_in_t *in;
	int i, j;

	for (i = 0; i < llist_size(transactions); i++)
	{
		tx = llist_get_node_at(transactions, i);
		for (j = 0; j < llist_size(tx->inputs); j++)
		{
			in = llist_get_node_at(tx->inputs, j);
			if (!memcmp(in->tx_out_hash, unspent->out.hash,
				SHA256_DIGEST_LENGTH))
				return (1);
		}
	}
	return (0);
}

/**
 * add_outputs - add the outputs of every transaction to a list of unspent
 *
 * @transactions: list of transactions
 * @block_hash: hash of the Block holding the transactions
 * @list: list to add to
 *
 * Return: 0 on success, -1 on failure
 */
static int add_outputs(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *list)
{
	transaction_t *tx;
	tx_out_t *out;
	unspent_tx_out_t *unspent;
	int i, j;

	for (i = 0; i < llist_size(transactions); i++)
	{
		tx = llist_get_node_at(transactions, i);
		for (j = 0; j < llist_size(tx->outputs); j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			unspent = unspent_tx_out_create(block_hash, tx->id, out);
			if (!unspent || llist_add_node(list, unspent, ADD_NODE_REAR))
			{
				free(unspent);
				return (-1);
			}
		}
	}
	return (0);
}

/**
 * clear_inputs - remove the inputs of every transaction once they are spent
 *
 * @transactions: list of transactions
 */
static void clear_inputs(llist_t *transactions)
{
	transaction_t *tx;
	tx_in_t *in;
	int i;

	for (i = 0; i < llist_size(transactions); i++)
	{
		tx = llist_get_node_at(transactions, i);
		in = llist_pop(tx->inputs);
		while (in)
		{
			free(in);
			in = llist_pop(tx->inputs);
		}
	}
}

/**
 * update_unspent - update the list of all unspent transaction outputs
 *
 * @transactions: list of validated transactions
 * @block_hash: hash of the validated Block holding the transactions
 * @all_unspent: current list of unspent transaction outputs
 *
 * Return: new list of unspent transaction outputs, or NULL on failure
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	llist_t *new_list;
	unspent_tx_out_t *unspent;
	int i;

	if (!transactions || !block_hash || !all_unspent)
		return (NULL);
	new_list = llist_create(MT_SUPPORT_FALSE);
	if (!new_list)
		return (NULL);

	for (i = 0; i < llist_size(all_unspent); i++)
	{
		unspent = llist_get_node_at(all_unspent, i);
		if (!is_spent(transactions, unspent))
		{
			unspent = unspent_tx_out_create(unspent->block_hash,
				unspent->tx_id, &unspent->out);
			if (!unspent || llist_add_node(new_list, unspent, ADD_NODE_REAR))
			{
				free(unspent);
				llist_destroy(new_list, 1, free);
				return (NULL);
			}
		}
	}
	if (add_outputs(transactions, block_hash, new_list) == -1)
	{
		llist_destroy(new_list, 1, free);
		return (NULL);
	}
	clear_inputs(transactions);
	llist_destroy(all_unspent, 1, free);
	return (new_list);
}
