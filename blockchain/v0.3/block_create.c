#include "blockchain.h"

/**
* block_create - Creates a Block structure and initializes it
* @prev: Pointer to the previous Block in the Blockchain
* @data: Points to a memory area to duplicate in the Block’s data
* @data_len: Number of bytes to duplicate in data
*
* Return: A pointer to the allocated Block
*/
block_t *block_create(block_t const *prev, int8_t const *data,
						uint32_t data_len)
{
	block_t *block;
	uint32_t max_len = data_len > BLOCKCHAIN_DATA_MAX ?
		BLOCKCHAIN_DATA_MAX : data_len;
	llist_t *list_tx = llist_create(MT_SUPPORT_FALSE);

	if (!prev || !data)
		return (NULL);
	block = calloc(1, sizeof(*block));
	if (!block || !list_tx)
	{
		free(block);
		llist_destroy(list_tx, 0, NULL);
		return (NULL);
	}

	memcpy(block->data.buffer, data, max_len);
	block->data.len = max_len;

	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	block->info.index = prev->info.index + 1;
	block->info.timestamp = (uint64_t)time(NULL);
	block->transactions = list_tx;

	return (block);
}
