#include "cli.h"


/**
* handle_info_block - handle the info_block command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/
/**
* handle_info_block - handle the info_block command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/
int handle_info_block(state_t *state)
{
	blockchain_t *blockchain = state->blockchain;
	llist_t *chain = blockchain->chain;
	block_t *block = NULL;
	unsigned int num_blocks;
	unsigned int i;
	unsigned int num_transactions;
	size_t block_size;
	size_t transactions_size;
	size_t total_size;

	if (!chain)
	{
		printf("No blocks in the blockchain\n");
		return (-1);
	}

	printf("Blockchain Information:\n");

	num_blocks = llist_size(chain);
	for (i = 0; i < num_blocks; i++)
	{
		block = (block_t *)llist_get_node_at(chain, i);
		if (block != NULL) {
			printf("Block Index: %u\n", block->info.index);
			printf("Timestamp: %lu\n", block->info.timestamp);
			printf("Previous Block Hash: %s\n", bytes_to_hex(block->info.prev_hash, SHA256_DIGEST_LENGTH));
			printf("Block Hash: %s\n", bytes_to_hex(block->hash, SHA256_DIGEST_LENGTH));

			num_transactions= llist_size(block->transactions);
			printf("Number of Transactions: %u\n", num_transactions);

			printf("List of Transactions:\n");

			llist_for_each(block->transactions, print_transaction_info, state->blockchain);

			block_size = sizeof(block_t);
			transactions_size = num_transactions * sizeof(transaction_t);
			total_size = block_size + transactions_size;
			printf("Block Size: %lu bytes\n", total_size);

			printf("\n");
		}
	}

	return (0);
}

/**
* print_transaction_info - print transaction information
* @node: transaction node
* @idx: index
* @arg: argument
* Return: 0
*/
int print_transaction_info(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *transaction;
	blockchain_t *blockchain = (blockchain_t *)arg;
	(void)idx;

	transaction = (transaction_t *)llist_get_node_data(node);
	printf("- Transaction ID: %s\n", bytes_to_hex(transaction->id, SHA256_DIGEST_LENGTH));

	printf("  Inputs:\n");
	llist_for_each(transaction->inputs, print_input_info, NULL);

	printf("  Outputs:\n");
	llist_for_each(transaction->outputs, print_output_info, NULL);

	printf("  Unspent:\n");

	if (blockchain)
		llist_for_each(blockchain->unspent, print_unspent_tx_out_info, NULL);
	return (0);
}

/**
* print_input_info - print input information
* @node: input node
* @idx: index
* @arg: argument
* Return: 0
*/
int print_input_info(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *input;
	(void)idx;
	(void)arg;

	input = (tx_in_t *)llist_get_node_data(node);
	printf("    - Block Hash: %s\n", bytes_to_hex(input->block_hash, SHA256_DIGEST_LENGTH));
	printf("    - Transaction ID: %s\n", bytes_to_hex(input->tx_id, SHA256_DIGEST_LENGTH));
	printf("    - Transaction Output Hash: %s\n", bytes_to_hex(input->tx_out_hash, SHA256_DIGEST_LENGTH));
	return (0);
}

/**
* print_output_info - print output information
* @node: output node
* @idx: index
* @arg: argument
* Return: 0
*/
int print_output_info(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *output;
	(void)idx;
	(void)arg;

	output = (tx_out_t *)llist_get_node_data(node);
	printf("    - Amount: %u\n", output->amount);
	printf("    - Public Address: %s\n", bytes_to_hex(output->pub, EC_PUB_LEN));
	printf("    - Output Hash: %s\n", bytes_to_hex(output->hash, SHA256_DIGEST_LENGTH));
	return (0);
}
