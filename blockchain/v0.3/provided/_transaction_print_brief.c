#include "transaction.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _transaction_print_brief_loop - print a transaction on a few lines
 *
 * @transaction: transaction to print
 * @idx: unused
 * @indent: indentation
 *
 * Return: 0
 */
int _transaction_print_brief_loop(transaction_t const *transaction,
	unsigned int idx, char const *indent)
{
	tx_out_t const *out;
	uint32_t amount = 0;
	int i;

	for (i = 0; i < llist_size(transaction->outputs); i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		amount += out->amount;
	}
	printf("%sTransaction: {\n", indent);
	printf("%s\tamount: %u from %d inputs,\n", indent, amount,
		llist_size(transaction->inputs));
	printf("%s\treceiver: ", indent);
	out = llist_get_head(transaction->outputs);
	if (out)
		_print_hex_buffer(out->pub, EC_PUB_LEN);
	printf("\n%s\tid: ", indent);
	_print_hex_buffer(transaction->id, SHA256_DIGEST_LENGTH);
	printf("\n%s}\n", indent);
	(void)idx;
	return (0);
}

/**
 * _transaction_print_brief - print a transaction on a few lines
 *
 * @transaction: transaction to print
 */
void _transaction_print_brief(transaction_t const *transaction)
{
	if (transaction)
		_transaction_print_brief_loop(transaction, 0, "");
}
