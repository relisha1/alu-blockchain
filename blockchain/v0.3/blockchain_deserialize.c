#include "blockchain.h"

#define CHECK_ENDIAN(x) (swap ? SWAPENDIAN(x) : (void)0)
#define read_all(fd, buf, len) \
	(read(fd, buf, len) == (ssize_t)(len) ? 0 : -1)

/**
 * read_tx_out - read one transaction output
 * @fd: file descriptor
 * @out: destination
 * @swap: non-zero if byte order must be swapped
 * Return: 0 on success, -1 on failure
 */
static int read_tx_out(int fd, tx_out_t *out, uint8_t swap)
{
	if (read_all(fd, &out->amount, sizeof(out->amount)) ||
	    read_all(fd, out->pub, sizeof(out->pub)) ||
	    read_all(fd, out->hash, sizeof(out->hash)))
		return (-1);
	CHECK_ENDIAN(out->amount);
	return (0);
}

/**
 * read_transaction - read one transaction
 * @fd: file descriptor
 * @swap: non-zero if byte order must be swapped
 * Return: new transaction or NULL
 */
static transaction_t *read_transaction(int fd, uint8_t swap)
{
	transaction_t *tx = calloc(1, sizeof(*tx));
	int32_t nb_in, nb_out, i;
	tx_in_t *in;
	tx_out_t *out;

	if (!tx)
		return (NULL);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!tx->inputs || !tx->outputs || read_all(fd, tx->id, sizeof(tx->id)) ||
	    read_all(fd, &nb_in, 4) || read_all(fd, &nb_out, 4))
		return (transaction_destroy(tx), NULL);
	CHECK_ENDIAN(nb_in);
	CHECK_ENDIAN(nb_out);
	for (i = 0; i < nb_in; i++)
	{
		in = calloc(1, sizeof(*in));
		if (!in || read_all(fd, in->block_hash, sizeof(in->block_hash)) ||
		    read_all(fd, in->tx_id, sizeof(in->tx_id)) ||
		    read_all(fd, in->tx_out_hash, sizeof(in->tx_out_hash)) ||
		    read_all(fd, in->sig.sig, sizeof(in->sig.sig)) ||
		    read_all(fd, &in->sig.len, sizeof(in->sig.len)) ||
		    llist_add_node(tx->inputs, in, ADD_NODE_REAR))
			return (free(in), transaction_destroy(tx), NULL);
	}
	for (i = 0; i < nb_out; i++)
	{
		out = calloc(1, sizeof(*out));
		if (!out || read_tx_out(fd, out, swap) ||
		    llist_add_node(tx->outputs, out, ADD_NODE_REAR))
			return (free(out), transaction_destroy(tx), NULL);
	}
	return (tx);
}

/**
 * read_block - read one block and its transactions
 * @fd: file descriptor
 * @swap: non-zero if byte order must be swapped
 * Return: new block or NULL
 */
static block_t *read_block(int fd, uint8_t swap)
{
	block_t *block = calloc(1, sizeof(*block));
	int32_t nb_tx, i;
	transaction_t *tx;

	if (!block || read_all(fd, &block->info, sizeof(block->info)) ||
	    read_all(fd, &block->data.len, 4))
		return (free(block), NULL);
	CHECK_ENDIAN(block->info.index);
	CHECK_ENDIAN(block->info.difficulty);
	CHECK_ENDIAN(block->info.timestamp);
	CHECK_ENDIAN(block->info.nonce);
	CHECK_ENDIAN(block->data.len);
	if (block->data.len > BLOCKCHAIN_DATA_MAX ||
	    read_all(fd, block->data.buffer, block->data.len) ||
	    read_all(fd, block->hash, SHA256_DIGEST_LENGTH) ||
	    read_all(fd, &nb_tx, 4))
		return (free(block), NULL);
	CHECK_ENDIAN(nb_tx);
	if (nb_tx < 0) /* genesis block has no transaction list */
		return (block);
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
		return (free(block), NULL);
	for (i = 0; i < nb_tx; i++)
	{
		tx = read_transaction(fd, swap);
		if (!tx || llist_add_node(block->transactions, tx, ADD_NODE_REAR))
			return (transaction_destroy(tx), block_destroy(block), NULL);
	}
	return (block);
}

/**
 * read_header - read and check the file header
 * @fd: file descriptor
 * @swap: set to non-zero if byte order must be swapped
 * @nb_blocks: number of blocks
 * @nb_unspent: number of unspent outputs
 * Return: 0 on success, -1 on failure
 */
static int read_header(int fd, uint8_t *swap, int32_t *nb_blocks,
		       int32_t *nb_unspent)
{
	char buf[8] = {0};
	uint8_t endian;

	if (read_all(fd, buf, strlen(HBLK_MAGIC)) || strcmp(buf, HBLK_MAGIC))
		return (-1);
	memset(buf, 0, sizeof(buf));
	if (read_all(fd, buf, strlen(HBLK_VERSION)) || strcmp(buf, HBLK_VERSION))
		return (-1);
	if (read_all(fd, &endian, 1) || read_all(fd, nb_blocks, 4) ||
	    read_all(fd, nb_unspent, 4))
		return (-1);
	*swap = endian != _get_endianness();
	if (*swap)
	{
		SWAPENDIAN(*nb_blocks);
		SWAPENDIAN(*nb_unspent);
	}
	return (*nb_blocks < 1 || *nb_unspent < 0 ? -1 : 0);
}

/**
 * blockchain_deserialize - deserializes blockchain from file
 * @path: path to serialized blockchain file
 * Return: pointer to deserialized blockchain or NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	int fd;
	uint8_t swap;
	int32_t nb_blocks, nb_unspent, i;
	blockchain_t *chain;
	block_t *block;
	unspent_tx_out_t *u;

	if (!path)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	if (read_header(fd, &swap, &nb_blocks, &nb_unspent))
		return (close(fd), NULL);
	chain = calloc(1, sizeof(*chain));
	if (!chain)
		return (close(fd), NULL);
	chain->chain = llist_create(MT_SUPPORT_TRUE);
	chain->unspent = llist_create(MT_SUPPORT_TRUE);
	if (!chain->chain || !chain->unspent)
		return (blockchain_destroy(chain), close(fd), NULL);
	for (i = 0; i < nb_blocks; i++)
	{
		block = read_block(fd, swap);
		if (!block || llist_add_node(chain->chain, block, ADD_NODE_REAR))
			return (block_destroy(block), blockchain_destroy(chain),
				close(fd), NULL);
	}
	for (i = 0; i < nb_unspent; i++)
	{
		u = calloc(1, sizeof(*u));
		if (!u || read_all(fd, u->block_hash, sizeof(u->block_hash)) ||
		    read_all(fd, u->tx_id, sizeof(u->tx_id)) ||
		    read_tx_out(fd, &u->out, swap) ||
		    llist_add_node(chain->unspent, u, ADD_NODE_REAR))
			return (free(u), blockchain_destroy(chain), close(fd), NULL);
	}
	return (close(fd), chain);
}
