#include "transaction.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _tx_in_print - print a transaction input
 *
 * @in: transaction input
 * @idx: unused
 * @indent: indentation
 *
 * Return: 0
 */
static int _tx_in_print(tx_in_t const *in, unsigned int idx,
	char const *indent)
{
	printf("%s{\n", indent);
	printf("%s\tblock_hash: ", indent);
	_print_hex_buffer(in->block_hash, sizeof(in->block_hash));
	printf(",\n%s\ttx_id: ", indent);
	_print_hex_buffer(in->tx_id, sizeof(in->tx_id));
	printf(",\n%s\ttx_out_hash: ", indent);
	_print_hex_buffer(in->tx_out_hash, sizeof(in->tx_out_hash));
	printf(",\n%s\tsig: ", indent);
	if (in->sig.len)
		_print_hex_buffer(in->sig.sig, in->sig.len);
	else
		printf("null");
	printf("\n%s}\n", indent);
	(void)idx;
	return (0);
}

/**
 * _tx_out_print - print a transaction output
 *
 * @out: transaction output
 * @idx: unused
 * @indent: indentation
 *
 * Return: 0
 */
static int _tx_out_print(tx_out_t const *out, unsigned int idx,
	char const *indent)
{
	printf("%s{\n", indent);
	printf("%s\tamount: %u,\n", indent, out->amount);
	printf("%s\tpub: ", indent);
	_print_hex_buffer(out->pub, sizeof(out->pub));
	printf(",\n%s\thash: ", indent);
	_print_hex_buffer(out->hash, sizeof(out->hash));
	printf("\n%s}\n", indent);
	(void)idx;
	return (0);
}

/**
 * _transaction_print - print a transaction
 *
 * @transaction: transaction to print
 */
void _transaction_print(transaction_t const *transaction)
{
	if (!transaction)
		return;
	printf("Transaction: {\n");
	printf("\tinputs [%d]: [\n", llist_size(transaction->inputs));
	llist_for_each(transaction->inputs, (node_func_t)_tx_in_print, "\t\t");
	printf("\t],\n");
	printf("\toutputs [%d]: [\n", llist_size(transaction->outputs));
	llist_for_each(transaction->outputs, (node_func_t)_tx_out_print, "\t\t");
	printf("\t],\n");
	printf("\tid: ");
	_print_hex_buffer(transaction->id, sizeof(transaction->id));
	printf("\n}\n");
}
