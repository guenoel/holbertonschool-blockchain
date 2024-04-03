#include "transaction.h"

/**
 * coinbase_is_valid - validates a coinbase transaction
 *
 * @coinbase: coinbase transaction to validate
 * @block_index: index of the Block containing coinbase transaction
 *
 * Return: 1 if valid, 0 if not
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash_buff[SHA256_DIGEST_LENGTH];
	tx_in_t *input;
	tx_out_t *output;

	if (!coinbase)
		return (0);
	transaction_hash(coinbase, hash_buff);
	if (memcmp(coinbase->id, hash_buff, SHA256_DIGEST_LENGTH))
		return (0);
	if (llist_size(coinbase->inputs) != 1)
		return (0);
	if (llist_size(coinbase->outputs) != 1)
		return (0);
	input = llist_get_node_at(coinbase->inputs, 0);
	if (memcmp(input->tx_out_hash, &block_index, 4))
		return (0);
	if (input->block_hash || input->tx_id || input->sig.sig || input->sig.len)
		return (0);
	output = llist_get_node_at(coinbase->outputs, 0);
	if (output->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
