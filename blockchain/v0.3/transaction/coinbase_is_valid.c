#include "transaction.h"

/**
* is_empty - checks if buffer memory is all 0
* @buf: the buffer to check
* @size: size of buffer
* Return: 1 if empty else 0
*/
int is_empty(uint8_t *buf, size_t size)
{
	/* Check if buffer memory is all 0 */
	size_t i;

	/* Iterate through buffer */
	for (i = 0; i < size; i++)
	{
		/* If not 0, return 0 */
		if (buf[i] != 0)
			return (0);
	}
	/* If all 0, return 1 */
	return (1);
}

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
	if (!is_empty(input->block_hash, sizeof(input->block_hash)) ||
				!is_empty(input->tx_id, sizeof(input->tx_id)) ||
				!is_empty((uint8_t *)&input->sig, sizeof(input->sig)))
		return (0);

	output = llist_get_node_at(coinbase->outputs, 0);
	if (output->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
