#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <llist.h>
#include "hblk_crypto.h"

#define COINBASE_AMOUNT 50

/**
 * struct transaction_s - Transaction structure
 * @id: transaction ID, a hash of all the inputs and outputs
 * @inputs: list of tx_in_t *
 * @outputs: list of tx_out_t *
 */
typedef struct transaction_s
{
	uint8_t id[SHA256_DIGEST_LENGTH];
	llist_t *inputs;
	llist_t *outputs;
} transaction_t;

/**
 * struct tx_out_s - Transaction output
 * @amount: amount received
 * @pub: receiver's public address
 * @hash: hash of @amount and @pub, serves as output ID
 */
typedef struct tx_out_s
{
	uint32_t amount;
	uint8_t pub[EC_PUB_LEN];
	uint8_t hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * struct tx_in_s - Transaction input
 * @block_hash: hash of the block containing the referenced output
 * @tx_id: ID of the transaction containing the referenced output
 * @tx_out_hash: hash of the referenced output
 * @sig: signature, prevents anyone from altering the transaction
 */
typedef struct tx_in_s
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	uint8_t tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t sig;
} tx_in_t;

/**
 * struct unspent_tx_out_s - Unspent transaction output
 * @block_hash: hash of the block containing the transaction @tx_id
 * @tx_id: ID of the transaction containing @out
 * @out: copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t out;
} unspent_tx_out_t;

/**
 * struct visitor_s - state used while collecting a sender's coins
 * @sender_unspent: unspent outputs owned by the sender
 * @sender_pub: sender's public key
 * @amount: amount to send
 * @total_amount: sum of @sender_unspent
 */
typedef struct visitor_s
{
	llist_t *sender_unspent;
	uint8_t *sender_pub;
	uint32_t amount;
	unsigned long total_amount;
} visitor_t;

/**
 * struct validation_vistor_s - state used while validating
 * @valid: 1 while everything checked so far is valid
 * @all_unspent: list of all unspent outputs
 * @block_index: index of the block being validated
 * @tx: transaction being validated
 * @in_amount: sum of input amounts
 * @out_amount: sum of output amounts
 */
typedef struct validation_vistor_s
{
	int valid;
	llist_t *all_unspent;
	uint32_t block_index;
	transaction_t const *tx;
	unsigned long in_amount;
	unsigned long out_amount;
} validation_vistor_t;

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *tx_out);
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);
uint8_t *transaction_hash(transaction_t const *transaction,
			  uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		  EC_KEY const *sender, llist_t *all_unspent);
transaction_t *transaction_create(EC_KEY const *sender,
				  EC_KEY const *receiver, uint32_t amount,
				  llist_t *all_unspent);
int transaction_is_valid(transaction_t const *transaction,
			 llist_t *all_unspent);
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);
void transaction_destroy(transaction_t *transaction);
llist_t *update_unspent(llist_t *transactions,
			uint8_t block_hash[SHA256_DIGEST_LENGTH],
			llist_t *all_unspent);

#endif /* _TRANSACTION_H_ */
