#include "blockchain.h"

/**
 * block_hash - computes the hash of a Block
 * @block: pointer to the Block to be hashed
 * @hash_buf: buffer to store the hash
 * Return: a pointer to hash_buf
 */
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;

	if (!block)
		return (NULL);
	len = sizeof(block->info) + block->data.len;
	return (sha256((int8_t const *)block, len, hash_buf));
}
