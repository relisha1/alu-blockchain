#include "blockchain.h"

/**
 * write_all - write exactly len bytes
 * @fd: file descriptor
 * @buf: data
 * @len: number of bytes
 * Return: 0 on success, -1 on failure
 */
static int write_all(int fd, void const *buf, size_t len)
{
	return (write(fd, buf, len) == (ssize_t)len ? 0 : -1);
}

/**
 * write_tx_out - serialize one transaction output
 * @fd: file descriptor
 * @out: output
 * Return: 0 on success, -1 on failure
 */
static int write_tx_out(int fd, tx_out_t const *out)
{
	if (write_all(fd, &out->amount, sizeof(out->amount)) ||
	    write_all(fd, out->pub, sizeof(out->pub)) ||
	    write_all(fd, out->hash, sizeof(out->hash)))
		return (-1);
	return (0);
}

/**
 * write_transaction - serialize one transaction
 * @fd: file descriptor
 * @tx: transaction
 * Return: 0 on success, -1 on failure
 */
static int write_transaction(int fd, transaction_t const *tx)
{
	int32_t nb_in = llist_size(tx->inputs), nb_out = llist_size(tx->outputs);
	int32_t i;
	tx_in_t *in;

	if (nb_in < 0 || nb_out < 0)
		return (-1);
	if (write_all(fd, tx->id, sizeof(tx->id)) ||
	    write_all(fd, &nb_in, 4) || write_all(fd, &nb_out, 4))
		return (-1);
	for (i = 0; i < nb_in; i++)
	{
		in = llist_get_node_at(tx->inputs, i);
		if (!in || write_all(fd, in->block_hash, sizeof(in->block_hash)) ||
		    write_all(fd, in->tx_id, sizeof(in->tx_id)) ||
		    write_all(fd, in->tx_out_hash, sizeof(in->tx_out_hash)) ||
		    write_all(fd, in->sig.sig, sizeof(in->sig.sig)) ||
		    write_all(fd, &in->sig.len, sizeof(in->sig.len)))
			return (-1);
	}
	for (i = 0; i < nb_out; i++)
		if (write_tx_out(fd, llist_get_node_at(tx->outputs, i)))
			return (-1);
	return (0);
}

/**
 * write_block - serialize one block and its transactions
 * @fd: file descriptor
 * @block: block
 * Return: 0 on success, -1 on failure
 */
static int write_block(int fd, block_t const *block)
{
	int32_t nb_tx = block->transactions ? llist_size(block->transactions) : -1;
	int32_t i;

	if (write_all(fd, &block->info, sizeof(block->info)) ||
	    write_all(fd, &block->data.len, 4) ||
	    write_all(fd, block->data.buffer, block->data.len) ||
	    write_all(fd, block->hash, SHA256_DIGEST_LENGTH) ||
	    write_all(fd, &nb_tx, 4))
		return (-1);
	for (i = 0; i < nb_tx; i++)
		if (write_transaction(fd, llist_get_node_at(block->transactions, i)))
			return (-1);
	return (0);
}

/**
 * blockchain_serialize - serializes blockchain to file
 * @blockchain: pointer to blockchain to serialize
 * @path: path to save file
 * Return: 0 on success else -1 on failure.
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd;
	int32_t i, nb_blocks, nb_unspent;
	uint8_t endianness = _get_endianness();
	unspent_tx_out_t *u;

	if (!blockchain || !blockchain->chain || !path)
		return (-1);
	nb_blocks = llist_size(blockchain->chain);
	nb_unspent = blockchain->unspent ? llist_size(blockchain->unspent) : 0;
	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd == -1)
		return (-1);
	if (write_all(fd, HBLK_MAGIC, strlen(HBLK_MAGIC)) ||
	    write_all(fd, HBLK_VERSION, strlen(HBLK_VERSION)) ||
	    write_all(fd, &endianness, 1) || write_all(fd, &nb_blocks, 4) ||
	    write_all(fd, &nb_unspent, 4))
		return (close(fd), -1);
	for (i = 0; i < nb_blocks; i++)
		if (write_block(fd, llist_get_node_at(blockchain->chain, i)))
			return (close(fd), -1);
	for (i = 0; i < nb_unspent; i++)
	{
		u = llist_get_node_at(blockchain->unspent, i);
		if (!u || write_all(fd, u->block_hash, sizeof(u->block_hash)) ||
		    write_all(fd, u->tx_id, sizeof(u->tx_id)) ||
		    write_tx_out(fd, &u->out))
			return (close(fd), -1);
	}
	return (close(fd), 0);
}
