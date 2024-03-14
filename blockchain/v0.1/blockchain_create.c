#include "blockchain.h"

/**
 * blockchain_create - creates a Blockchain structure, and initializes it
 *
 * Return: a pointer to the created blockchain, or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = calloc(1, sizeof(*blockchain));
	block_t *genesis = calloc(1, sizeof(*genesis));
	llist_t *list = NULL;

	if (!blockchain || !genesis)
	{
		free(blockchain);
		free(genesis);
		return (NULL);
	}

	list = llist_create(MT_SUPPORT_TRUE);
	if (!list)
	{
		free(blockchain);
		free(genesis);
		return (NULL);
	}

	genesis->info.index = 0;
	genesis->info.difficulty = 0;
	genesis->info.timestamp = 1537578000;
	genesis->info.nonce = 0;
	memset(genesis->info.prev_hash, 0, SHA256_DIGEST_LENGTH);

	memcpy(genesis->data.buffer, "Holberton School", 16);
	genesis->data.len = 16;
	memcpy(genesis->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);

	if (llist_add_node(list, genesis, ADD_NODE_FRONT) == -1)
	{
		free(blockchain);
		free(genesis);
		llist_destroy(list, 1, NULL);
		return (NULL);
	}

	blockchain->chain = list;

	return (blockchain);
}
