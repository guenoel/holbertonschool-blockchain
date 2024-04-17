#include "cli.h"

/**
* invalid_tx - filter invalid transactions
* @node: transaction
* @arg: list of unspent transactions
* Return: 1 if node is invalid,0 otherwise
*
*/
int invalid_tx(llist_node_t node, void *arg)
{
	transaction_t *transaction = node;
	llist_t *unspent = arg;

	return (!transaction_is_valid(transaction, unspent));
}

/**
* add_transaction - add a transaction to a block
* @node: transaction
* @idx: transaction index
* @arg: block in wich the transaction will be added
* Return: 0 on succes -1 otherwise
*/
int add_transaction(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *transaction = node;
	block_t *block = arg;
	(void)idx;

	return (llist_add_node(block->transactions, transaction, ADD_NODE_REAR));
}

/**
* mine - mine a block
* @state: cli state
* @block: block to mine
* @prev_block: previous block in the blockchain
* @coinbase_tx: coinbase transaction
* Return: EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
int mine(state_t *state, block_t *block, block_t *prev_block,
		transaction_t *coinbase_tx)
{
	int size, i;
	unspent_tx_out_t *utxo = NULL;

	size = llist_size(state->tx_pool);
	for (i = 0; i < size; i++)
		llist_remove_node(state->tx_pool, invalid_tx, state->blockchain->unspent, 1,
				(node_dtor_t)transaction_destroy);

	/* add all tx from pool to block*/
	llist_for_each(state->tx_pool, add_transaction, block);
	block->info.difficulty = blockchain_difficulty(state->blockchain);
	/* add coinbase tx to block */
	llist_add_node(block->transactions, coinbase_tx, ADD_NODE_FRONT);
	/* mine block and check if block is valid */
	block_mine(block);
	if ((block_is_valid(block, prev_block, state->blockchain->unspent)) != 0)
	{
		fprintf(stderr, "mine: Failed to mine a valid block\n");
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return (-1);
	}
	/* update unspent from transaction list */
	state->blockchain->unspent = update_unspent(block->transactions,
					block->hash, state->blockchain->unspent);
	/* //TODO: c'est pas plutot dans transaction de mettre le coinbase */
	/* puis ensuite faire un unspent_update pour que ca arrive dans unspent */
	/* create unspent tx from coinbase tx */
	utxo = unspent_tx_out_create(block->hash, coinbase_tx->id,
				llist_get_head(coinbase_tx->outputs));
	if (!utxo)
	{
		fprintf(stderr, "mine: failed to create unspent tx output\n");
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return (-1);
	}
	while (llist_pop(state->tx_pool))
		;
	/* add block to blockchain */
	llist_add_node(state->blockchain->chain, block, ADD_NODE_REAR);
	/* add unspent tx to unspent list */
	llist_add_node(state->blockchain->unspent, utxo, ADD_NODE_REAR);
	fprintf(stdout, "Block Successfuly mined\n");
	return (0);
}
