#include "blockchain.h"

/**
* blockchain_create - Creates a Blockchain structure and initializes it
*
* Return: A pointer to the created blockchain, or NULL on failure
*/
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = calloc(1, sizeof(blockchain_t));
	block_t *genesis_block = calloc(1, sizeof(block_t));
	/* If a memory allocation fails, return NULL */
	if (!blockchain)
		return (NULL);
	blockchain->chain = llist_create(MT_SUPPORT_TRUE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}
	if (!genesis_block)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}
	memset(genesis_block, 0, sizeof(block_t));/* Initialize the genesis block */
	genesis_block->info.index = 0;
	genesis_block->info.difficulty = 0;
	genesis_block->info.timestamp = 1537578000;
	genesis_block->info.nonce = 0;
	memset(genesis_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
	memcpy(genesis_block->data.buffer, "Holberton School", 16);
	/* Set the data length to 16 */
	genesis_block->data.len = 16;
	/* Set the hash to the genesis hash */
	memcpy(genesis_block->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);
	/* Add the genesis block to the blockchain */
	llist_add_node(blockchain->chain, genesis_block, ADD_NODE_FRONT);
	return (blockchain);
}
