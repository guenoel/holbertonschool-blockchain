#include "blockchain.h"

/**
 * blockchain_destroy - deletes an existing blockchain
 * @blockchain: pointer to the blockchain to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (blockchain)
	{
		llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
		free(blockchain);
	}
}
