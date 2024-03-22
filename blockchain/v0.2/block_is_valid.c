#include "blockchain.h"

/**
* cleanup_hash - Clears a hash by setting all its bytes to 0
* @hash: Pointer to the hash to clear
* @hash_len: Length of the hash in bytes
*/
void cleanup_hash(uint8_t *hash, size_t hash_len)
{
	memset(hash, 0, hash_len);
}

/**
* block_is_valid - Verifies if a block is valid
* @block: Pointer to the block to check
* @prev_block: Pointer to the previous block in the blockchain
*
* Return: 1 if the block is valid, otherwise 0
*/
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_verify[SHA256_DIGEST_LENGTH] = {0};
	block_t const genesis = GENESIS_BLOCK;

	/* Check if block or previous block is null*/
	if (!block || (!prev_block && block->info.index != 0))
		return (1);
	/* Check if block is genesis block */
	if (block->info.index == 0)
		return (memcmp(block, &genesis, sizeof(genesis)));
	/* Check if the block index is correct in relation to the previous block */
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	/* Check if the hash of the previous block is the same as the previous hash */
	if (block_hash(prev_block, hash_verify) == NULL ||
		memcmp(hash_verify, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	/*Check if the previous hash is the same as the hash of the previous block */
	if (memcmp(prev_block->hash, block->info.prev_hash,
													SHA256_DIGEST_LENGTH) != 0)
		return (1);
	/* Clear the hash_verify */
	cleanup_hash(hash_verify, SHA256_DIGEST_LENGTH);
	/* Check if the block hash is the same as the stored block hash */
	if (block_hash(block, hash_verify) == NULL ||
		memcmp(hash_verify, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	/* Check if the data length is greater than the maximum allowed length*/
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	/* Check if the hash matches the difficulty */
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);

	return (0);
}
