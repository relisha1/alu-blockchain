#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <llist.h>
#include "hblk_crypto.h"
#include "provided/provided.h"
#include "transaction/transaction.h"

#define BLOCKCHAIN_DATA_MAX 1024
#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.3"

#define BLOCK_GENERATION_INTERVAL 1
#define DIFFICULTY_ADJUSTMENT_INTERVAL 5

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/**
 * struct blockchain_s - Blockchain structure
 * @chain: linked list of blocks (block_t *)
 * @unspent: linked list of unspent transaction outputs
 */
typedef struct blockchain_s
{
	llist_t *chain;
	llist_t *unspent;
} blockchain_t;

/**
 * struct block_info_s - Block info structure
 * @index: index of the block in the chain
 * @difficulty: difficulty of proof of work
 * @timestamp: time the block was created (UNIX timestamp)
 * @nonce: salt value used to alter the block hash
 * @prev_hash: hash of the previous block
 */
typedef struct block_info_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data
 * @buffer: data buffer
 * @len: data size in bytes
 */
typedef struct block_data_s
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t len;
} block_data_t;

/**
 * struct block_s - Block structure
 * @info: block info
 * @data: block data
 * @transactions: list of transactions (transaction_t *)
 * @hash: 256-bit digest of the block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t info;
	block_data_t data;
	llist_t *transactions;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

/* GENESIS_BLOCK - first block in the blockchain */
#define GENESIS_BLOCK { \
	{ /* info */ \
		0 /* index */, \
		0 /* difficulty */, \
		1537578000 /* timestamp */, \
		0 /* nonce */, \
		{0} /* prev_hash */ \
	}, \
	{ /* data */ \
		"Holberton School" /* buffer */, \
		16 /* len */ \
	}, \
	NULL /* transactions */, \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" \
}

blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block,
		   llist_t *all_unspent);
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

#endif /* _BLOCKCHAIN_H_ */
