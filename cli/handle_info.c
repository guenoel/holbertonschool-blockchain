#include "cli.h"

/**
* bytes_to_hex - converts an array of bytes to a hex string
* @bytes: array of bytes
* @len: number of bytes
* Return: pointer to the resulting hex string
*/
char *bytes_to_hex(const uint8_t *bytes, size_t len)
{
	size_t i;
	static const char hex_chars[] = "0123456789abcdef";
	char *hex_str = malloc(len * 2 + 1);

	if (hex_str)
	{
		for ( i = 0; i < len; i++)
		{
			hex_str[i * 2] = hex_chars[(bytes[i] >> 4) & 0xF];
			hex_str[i * 2 + 1] = hex_chars[bytes[i] & 0xF];
		}
		hex_str[len * 2] = '\0';
	}
	return (hex_str);
}

/**
* handle_info - handle the info command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/

int handle_info(state_t *state)
{
	blockchain_t *blockchain = state->blockchain;
	llist_t *unspent = blockchain->unspent;
	llist_t *tx_pool = state->tx_pool;
	uint32_t coins = 0;
	block_t *last_block = NULL;

	if (!state || !blockchain)
	{
		printf("Error: State or Blockchain is NULL\n");
		return (-1);
	}

	if (!state->blockchain)
	{
		printf("Error: Blockchain is NULL\n");
		return (-1);
	}

	llist_for_each(state->blockchain->unspent, sum_unspent, &coins);

	printf("Number of Blocks in Blockchain: %u\n", llist_size(blockchain->chain));
	printf("Number of unspent transaction outputs: %u\n", llist_size(unspent));

	if (tx_pool)
	{
		printf("Transaction pool is available\n");
		printf("Number of pending transactions in transaction pool: %u\n",
				llist_size(tx_pool));
	}
	else
	{
		printf("Error: Transaction pool is NULL\n");
	}
	printf("Coins: %u\n", coins);


	last_block = llist_get_tail(blockchain->chain);
	if (last_block)
	{
		/* Imprimir el encabezado del bloque */
		printf("\nBlock Header:\n");
		/*Verificar si el bloque actual es el bloque génesis*/
		if (last_block->info.index == 0)
		{
			printf("This is the Genesis Block\n");
		}
		else
		{
			printf("Nonce: %lu\n", last_block->info.nonce);
			printf("Difficulty: %u\n", last_block->info.difficulty);
		}
	}
	else
	{
		printf("No blocks in the blockchain\n");
	}

	return (0);
}
