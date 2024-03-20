#include "blockchain.h"

/**
* blockchain_serialize - serializes a Blockchain into a file
* @blockchain: Points to the blockchain to serialize
* @path: Contains the path to a file to serialize Blockchain into
*        must be overwritten if pointing to an existing file
* Return: 0 on Success, -1 upon Failure
*/
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd, number_of_blocks = 0;
	int idx;
	/* Get the endianness of the machine */
	uint8_t endian = _get_endianness();

	/* Check if blockchain and path are not NULL */
	if (!blockchain || !path)
		return (-1);

	/* Open file */
	fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
		return (-1);

	/* Get the number of blocks in the chain */
	number_of_blocks = llist_size(blockchain->chain);

	/* Write file header */
	write(fd, HBLK_MAGIC, strlen(HBLK_MAGIC));
	write(fd, HBLK_VERSION, strlen(HBLK_VERSION));
	write(fd, &endian, sizeof(endian));
	write(fd, &number_of_blocks, sizeof(number_of_blocks));

	/* Write block data */
	for (idx = 0; idx < number_of_blocks; idx++)
	{
		/* Get the block */
		block_t *block = llist_get_node_at(blockchain->chain, idx);

		write(fd, &(block->info), sizeof(block->info));
		write(fd, &block->data.len, sizeof(block->data.len));
		write(fd, block->data.buffer, block->data.len);
		write(fd, block->hash, SHA256_DIGEST_LENGTH);
	}

	close(fd);
	return (0);
}
