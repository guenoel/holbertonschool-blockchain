#include "transaction.h"

/**
 * transaction_create - Creates a transaction
 *
 * @sender:    Private key of the transaction sender
 * @receiver:  Public key of the transaction receiver
 * @amount:    Amount to send
 * @all_unspent: List of all unspent transaction outputs
 *
 * Return: If an error occurs, return NULL.
 *         Otherwise, return a pointer to the created transaction.
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
									uint32_t amount, llist_t *all_unspent)
{
	transaction_t *transaction = calloc(1, sizeof(*transaction));
	tx_in_t *tx_in = NULL;
	tx_out_t *tx_out = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};
	ssize_t index;
	unspent_tx_out_t *unspent_output;

	if (!transaction)
		return (NULL);
	if (!sender || !receiver || !all_unspent)
		return (free(transaction), NULL);
	if (!ec_to_pub(sender, pub))
		return (free(transaction), NULL);
	tx_in = tx_in_create(NULL);
	if (!tx_in)
		return (free(transaction), NULL);
	if (!llist_add_node(transaction->inputs, tx_in, ADD_NODE_REAR))
		return (free(transaction), NULL);
	tx_out = tx_out_create(amount, pub);
	if (!tx_out)
		return (free(transaction), NULL);
	if (!llist_add_node(transaction->outputs, tx_out, ADD_NODE_REAR))
		return (free(transaction), NULL);
	for (index = 0; index < llist_size(all_unspent); index++)
	{
		unspent_output = llist_get_node_at(all_unspent, index);
		if (!unspent_output)
			return (free(transaction), NULL);
		if (!memcmp(unspent_output->out.pub, pub, EC_PUB_LEN))
		{
			tx_in = tx_in_create(unspent_output);
			if (!tx_in)
				return (free(transaction), NULL);
			if (!llist_add_node(transaction->inputs, tx_in, ADD_NODE_REAR))
				return (free(transaction), NULL);
		}
	}
	if (!transaction_hash(transaction, transaction->id))
		return (free(transaction), NULL);
	return (transaction);
}
