#include "blockchain.h"

/**
* cleanup_block_data - cleans resources associated with blockchain blocks
* @block: puntero al bloque a liberar
* @list: puntero a la lista a destruir
* Return: void
*/
void cleanup_block_data(block_t *block, llist_t *list)
{
	if (block != NULL)
	{
		free(block);
	}
	if (list != NULL)
	{
		llist_destroy(list, 1, NULL);
	}
}

/**
* cleanup_blockchain_data - clear string memory and close file
* @chain: pointer to chain
* @fd: file descriptor
* Return: void
*/
blockchain_t *cleanup_blockchain_data(blockchain_t *chain, int fd)
{
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

/**
* blockchain_deserialize - deserializes blockchain from file
* @path: path to serialized blockchain file
* Return: pointer to deserialized blockchain or null
*/
blockchain_t *blockchain_deserialize(char const *path)
{
	int fd = -1;
	blockchain_t *chain = NULL;
	uint8_t endianness;
	char buf[4096] = {0};
	uint32_t nb_blocks;

	if (!path)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	if (read(fd, buf, strlen(HBLK_MAGIC)) != strlen(HBLK_MAGIC) ||
		strcmp(buf, HBLK_MAGIC))
		return (cleanup_blockchain_data(chain, fd));
	buf[strlen(HBLK_VERSION)] = 0;
	if (read(fd, buf, strlen(HBLK_VERSION)) != strlen(HBLK_VERSION) ||
		strcmp(buf, HBLK_VERSION))
		return (cleanup_blockchain_data(chain, fd));
	chain = calloc(1, sizeof(*chain));
	if (!chain)
		return (cleanup_blockchain_data(chain, fd));
	if (read(fd, &endianness, 1) != 1)
		return (cleanup_blockchain_data(chain, fd));
	endianness = endianness != _get_endianness();
	if (read(fd, &nb_blocks, 4) != 4)
		return (cleanup_blockchain_data(chain, fd));
	chain->chain = blocks(fd, nb_blocks, endianness);
	if (!chain)
		return (cleanup_blockchain_data(chain, fd));

	return (chain);
}

/**
* blocks - deserializes all the blocks in the file
* @fd: open fd to save file
* @nb_blocks: number of blocks in the file
* @endianness: if endianess needs switching
* Return: pointer to list of blocks or NULL
*/
llist_t *blocks(int fd, uint32_t nb_blocks, uint8_t endianness)
{
	block_t *block;
	llist_t *list = llist_create(MT_SUPPORT_TRUE);
	uint32_t i = 0;

	if (!list)
		return (NULL);
	for (i = 0; i < nb_blocks; i++)
	{
		block = calloc(1, sizeof(*block));
		if (!block)
			return (cleanup_block_data(block, list), NULL);
		if (read(fd, &(block->info), sizeof(block->info)) != sizeof(block->info))
			return (cleanup_block_data(block, list), NULL);
		if (endianness)
		{
			block->info.index = SWAPENDIAN32(block->info.index);
			block->info.difficulty = SWAPENDIAN32(block->info.difficulty);
			block->info.timestamp = SWAPENDIAN32(block->info.timestamp);
			block->info.nonce = SWAPENDIAN32(block->info.nonce);
		}
		if (read(fd, &(block->data.len), 4) != 4)
			return (cleanup_block_data(block, list), NULL);
		if (endianness)
			block->data.len = SWAPENDIAN32(block->data.len);
		if (read(fd, block->data.buffer, block->data.len) != block->data.len)
			return (cleanup_block_data(block, list), NULL);
		if (read(fd, block->hash, SHA256_DIGEST_LENGTH) != SHA256_DIGEST_LENGTH)
			return (cleanup_block_data(block, list), NULL);
		if (llist_add_node(list, block, ADD_NODE_REAR))
			return (cleanup_block_data(block, list), NULL);
	}
	return (list);
}
