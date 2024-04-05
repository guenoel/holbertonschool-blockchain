#include "blockchain.h"

/**
* hash_transaction_ids - Hashes all the transaction IDs in a linked list
* @node: Pointer to the current node in the linked list
* @idx: Index of the current node (unused)
* @arg: Pointer to the address to write the hashed IDs to
* Return: 0 if success, else 1
*/
int hash_transaction_ids(llist_node_t node, unsigned int idx, void *arg)
{
	/* Copy the transaction ID to the buffer */
	memcpy(*(uint8_t **)arg, ((transaction_t *)node)->id, SHA256_DIGEST_LENGTH);
	/* Move the buffer pointer forward by the length of a SHA256 hash */
	*(uint8_t **)arg += SHA256_DIGEST_LENGTH;
	return (0); /* Return 0 if success */
	(void)idx; /* Unused parameter */
}

/**
* block_hash - Computes the hash of a block
* @block: Pointer to the block to hash
* @hash_buf: Buffer to store the hash/digest
* Return: Pointer to the buffer containing the hash
*/
uint8_t *block_hash(block_t const *block,
										uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	/*
	* Calculate the total length of data to hash, including block info and
	* transaction IDs
	*/
	size_t len_info_data = sizeof(block->info) + block->data.len;
	size_t len_info_data_tx;
	int8_t *block_data_buffer, *cursor;

	/* Initialize length to the length of block info */
	len_info_data_tx = len_info_data;
	/* There are transactions in block, add their hash lengths the total length */
	if (llist_size(block->transactions) > 0)
		len_info_data_tx += llist_size(block->transactions) * SHA256_DIGEST_LENGTH;

	/* Allocate memory for the buffer */
	cursor = block_data_buffer = calloc(1, len_info_data_tx);
	if (!cursor)
		return (NULL); /* Return NULL if memory allocation fails */

	/* Copy block info to buffer */
	memcpy(cursor, &block->info, len_info_data);
	/* Move Cursor for buffer */
	cursor += len_info_data;

	/* Iterate through transactions, copying each hash and appending the buffer */
	llist_for_each(block->transactions, hash_transaction_ids, &cursor);

	/* Calculate SHA256 hash of the block data buffer and store it in hash_buf */
	sha256(block_data_buffer, len_info_data_tx, hash_buf);

	/* Free allocated memory */
	free(block_data_buffer);

	return (hash_buf); /* Return pointer to the hash buffer */
}
