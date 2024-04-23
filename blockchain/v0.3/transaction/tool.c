#include "transaction.h"

/**
* sign_transaction_inputs - signs transaction inputs
* @node: current node
* @idx: index of @node
* @args: arguments:
* args[0] = uint8_t tx_id[SHA256_DIGEST_LENGTH] (ID of the transaction)
* args[1] = EC_KEY const *sender (sender's private key)
* args[2] = llist_t *all_unspent (list of all unspent transactions)
* Return: 0 on success, 1 on failure
*/
int sign_transaction_inputs(llist_node_t node, unsigned int idx, void *args)
{
	void **ptr = args;
	tx_in_t *tx_in = node;

	(void)idx; /* Avoid the warning "unused parameter" */

	/* Sign the transaction input using the sender's private key */
	if (!tx_in_sign(tx_in, ptr[0], ptr[1], ptr[2]))
		return (1);

	return (0);
}
