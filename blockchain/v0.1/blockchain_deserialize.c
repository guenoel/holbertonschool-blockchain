#include "blockchain.h"


void *free_blockchain(blockchain_t *chain, int fd, int idx)
{
	llist_t *list = chain->chain;
	block_t *block = llist_get_node_at(chain->chain, idx);
	int i;

	for (i = idx; i >= 0; i--)
	{
		if (block != NULL)
		{
			free(block);
		}
	}
	if (list != NULL)
	{
		llist_destroy(list, 1, NULL);
	}
	if (chain != NULL)
	{
		free(chain);
	}
	if (fd >= 0)
	{
		close(fd);
	}
	return (NULL);
}



blockchain_t *blockchain_deserialize(char const *path)
{
	int fd = -1, byte_read = 0, i, nb_blocks;
	char buf[4096] = {0};
	llist_t *list = llist_create(MT_SUPPORT_TRUE); /* multithreading */
	blockchain_t *chain = NULL;
	uint8_t endianness;
	block_t *block;

	if (!path)
		return (free_blockchain(chain, fd, -1));
	if (!list)
		return (free_blockchain(chain, fd, -1));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (free_blockchain(chain, fd, -1));

	byte_read = read(fd, buf, 4);
	if (byte_read != 4 || strncmp(buf, HBLK_MAGIC, 4))
		return (free_blockchain(chain, fd, -1));
	byte_read = read(fd, buf, 3);
	if (byte_read != 3 || strncmp(buf, HBLK_VERSION, 3))
		return (free_blockchain(chain, fd, -1));
	if (read(fd, &endianness, 1) != 1)
		return (free_blockchain(chain, fd, -1));
	if (read(fd, &nb_blocks, 4) != 4)
		return (free_blockchain(chain, fd, -1));

	for (i = 0; i < nb_blocks; i++)
	{
		block = calloc(1, sizeof(*block));
		if (!block)
			return (free_blockchain(chain, fd, i));
		if (read(fd, &block->info, sizeof(block->info)) != sizeof(block->info))
			return (free_blockchain(chain, fd, i));
		if (endianness)
		{
			block->info.index = SWAPENDIAN32(block->info.index);
			block->info.difficulty = SWAPENDIAN32(block->info.difficulty);
			block->info.timestamp = SWAPENDIAN32(block->info.timestamp);
			block->info.nonce = SWAPENDIAN32(block->info.nonce);
		}
		if (read(fd, &block->data.len, 4) != 4)
			return (free_blockchain(chain, fd, i));
		if (endianness)
			block->data.len = SWAPENDIAN32(block->data.len);
		if (read(fd, block->data.buffer, block->data.len) != block->data.len)
			return (free_blockchain(chain, fd, i));
		if (read(fd, &block->hash, SHA256_DIGEST_LENGTH) != SHA256_DIGEST_LENGTH)
			return (free_blockchain(chain, fd, i));
		if (llist_add_node(list, block, ADD_NODE_REAR))
			return (free_blockchain(chain, fd, i));
	}
	chain->chain = list;
	return (chain);
}
