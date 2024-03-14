#include "blockchain.h"

block_t *block_create(block_t const *prev, int8_t const *data,
						uint32_t data_len)
{
	block_t *new_block = NULL;
	uint32_t len = data_len > BLOCKCHAIN_DATA_MAX ?
		BLOCKCHAIN_DATA_MAX : data_len;

	if (!prev || !data)
		return (NULL);
	new_block = calloc(1, sizeof(*new_block));
	if (!new_block)
		return (NULL);

	new_block->info.index = prev->info.index + 1;
	new_block->info.difficulty = 0;
	new_block->info.timestamp = time(NULL);
	new_block->info.nonce = 0;
	memcpy(new_block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	memcpy(new_block->data.buffer, data, data_len);
	new_block->data.len = len;
	memset(new_block->hash, 0, SHA256_DIGEST_LENGTH);

	return (new_block);
}
