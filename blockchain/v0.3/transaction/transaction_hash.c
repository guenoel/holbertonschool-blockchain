#include "transaction.h"

/**
* hash_inputs - llist action func to hash inputs
* @transaction_input: tx_in_t * struct
* @destination_buffer: pointer to address to write to
* @idx: index of node
* Return: 0 if success else 1
*/
int inputs(llist_node_t transaction_input, unsigned int idx,
				void *destination_buffer)
{
	if (!transaction_input || !destination_buffer)
		return (1); /* Error: invalid input pointers */

	/* Copy input transaction data to destination buffer */
	memcpy(*(uint8_t **)destination_buffer, transaction_input,
		SHA256_DIGEST_LENGTH * 3);

	/* Advance the pointer in the destination buffer for the next copy */
	*(uint8_t **)destination_buffer += SHA256_DIGEST_LENGTH * 3;
	return (0);

	(void)idx; /* Avoid warning "unused parameter" */
}

/**
* hash_outputs - llist action func to hash outputs
* @transaction_output: tx_out_t * struct
* @destination_buffer: pointer to address to write to
* @idx: index of node
* Return: 0 if success else 1
*/
int outputs(llist_node_t transaction_output, unsigned int idx,
				void *destination_buffer)
{
	if (!transaction_output || !destination_buffer)
		return (1); /* Error: invalid input pointers */

	/* Copy the hash of the output transaction to the destination buffer */
	memcpy(*(uint8_t **)destination_buffer,
		((tx_out_t *)transaction_output)->hash, SHA256_DIGEST_LENGTH);

	/* Advance the pointer in the destination buffer for the next copy */
	*(uint8_t **)destination_buffer += SHA256_DIGEST_LENGTH;
	return (0);

	(void)idx; /* Avoid warning "unused parameter" */
}

/**
 * transaction_hash - Computes the hash of a transaction
 *
 * @transaction: Pointer to the transaction to hash
 * @hash_buf:    Pointer to buffer to store the resulting hash
 *
 * Return: If transaction is NULL or an error occurs, return NULL.
 *         Otherwise, return a pointer to the resulting hash.
 */
uint8_t *transaction_hash(transaction_t const *transaction,
							uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	ssize_t len;
	uint8_t *temp_buf, *curr_position;

	if (!transaction || !hash_buf)
		return (NULL);

	len = SHA256_DIGEST_LENGTH * 3 * llist_size(transaction->inputs) +
		SHA256_DIGEST_LENGTH * llist_size(transaction->outputs);
	if (len <= 0)
		return (NULL);
	/* Allocates memory for the temporary buffer */
	temp_buf = curr_position = calloc(1, len);
	if (!temp_buf)
		return (NULL); /* Error: memory allocation failed */

	/* Copy input transaction data to temporary buffer */
	llist_for_each(transaction->inputs, inputs, &curr_position);

	/* Copy output transaction hashes to temporary buffer */
	llist_for_each(transaction->outputs, outputs, &curr_position);

	/* Calculates SHA-256 hash of the temporary buffer and saves hash_buf */
	if (!sha256((const int8_t *)temp_buf, len, hash_buf))
	{
		free(temp_buf);
		return (NULL); /* Error: hash calculation failed */
	}

	/* Free the temporary buffer memory */
	free(temp_buf);

	/* Returns the calculated hash */
	return (hash_buf);
}
