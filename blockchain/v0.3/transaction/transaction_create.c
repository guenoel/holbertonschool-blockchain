#include "transaction.h"

/**
* select_unspent_in - selects unspent transactions that match the sender's
* public key
* @node: current node
* @idx: index of @node
* @args: arguments:
* args[0] = uint8_t const pub[EC_PUB_LEN] (public key of the sender)
* args[1] = llist_t *inputs (list to store selected transaction inputs)
* args[2] = uint32_t *amount (total amount of selected unspent transactions)
* Return: 0 on success, 1 on failure
*/
int select_unspent_in(llist_node_t node, unsigned int idx, void *args)
{
	unspent_tx_out_t *unspent = node;/* Unspent transaction output */
	void **ptr = args;/* Array of arguments */
	tx_in_t *tx_in;/* Transaction input */
	llist_t *inputs = ptr[1];/* List to store selected transaction inputs */
	uint32_t *amount = ptr[2];/* Total amount of selected unspent transactions */

	(void)idx; /* Avoid the warning "unused parameter" */

	/* Check public key of the unspent trans matches the sender's public key*/
	if (!memcmp(unspent->out.pub, ptr[0], EC_PUB_LEN))
	{
		/* Create transaction input from selected unspent transaction output */
		tx_in = tx_in_create(unspent);
		/* Add the transaction input to the list */
		llist_add_node(inputs, tx_in, ADD_NODE_REAR);
		/* Update the total amount of selected unspent transactions */
		*amount += unspent->out.amount;
	}
	return (0);
}

llist_t *send_amount(EC_KEY const *sender, EC_KEY const *receiver,
					int32_t amount, uint32_t unspent_amount)
{
	llist_t *tx_list = NULL; /* List to store transaction outputs */
	uint32_t remaining_balance = unspent_amount - amount; /* Calculate remaining balance */
	uint8_t receiver_pub_key[EC_PUB_LEN]; /* Buffer for receiver's public key */
	tx_out_t *tx_out = calloc(1, sizeof(*tx_out));

	/* Create a list to store transaction outputs */
	tx_list = llist_create(MT_SUPPORT_FALSE);
	if (!tx_list)
		/* Failed to create the list, return NULL */
		return (NULL);

	/* Create a transaction output for the specified amount */
	ec_to_pub(receiver, receiver_pub_key);/* Get the public key of the receiver */
	tx_out = tx_out_create(amount, receiver_pub_key); /* Create the output */
	if (!tx_out)
	{
		/* Failed to create the output, clean up and return NULL */
		llist_destroy(tx_list, 1, NULL);
		return (NULL);
	}
	/* Add the output to the list */
	llist_add_node(tx_list, tx_out, ADD_NODE_REAR);
	/* If there is a remaining balance, create another transaction output */
	if (remaining_balance != 0)
	{
		uint8_t sender_pub_key[EC_PUB_LEN]; /* Buffer for sender's public key */
		/* Get the public key of the sender */
		ec_to_pub(sender, sender_pub_key);
		/* Create a transaction output for the remaining balance */
		tx_out = tx_out_create(remaining_balance, sender_pub_key);
		if (!tx_list)
		{
			/* Failed to create the output, clean up and return NULL */
			llist_destroy(tx_list, 1, NULL);
			return (NULL);
		}
		/* Add the output to the list */
		llist_add_node(tx_list, tx_out, ADD_NODE_REAR);
	}

	return (tx_list);
}

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
	llist_t *tx_in = NULL;
	llist_t *tx_out = NULL;
	uint8_t pub[EC_PUB_LEN] = {0};
	void *args[3];
	uint32_t unspent_amount = 0;

	if (!transaction)
		return (NULL);
	if (!sender || !receiver || !all_unspent)
		return (free(transaction), NULL);
	/* Create the transaction inputs */
	tx_in = llist_create(MT_SUPPORT_FALSE);
	/* Get the public key of the sender */
	ec_to_pub(sender, pub);
	/* Select unspent transactions */
	args[0] = pub, args[1] = tx_in, args[2] = &unspent_amount;
	llist_for_each(all_unspent, select_unspent_in, args);
	/* Check if the sender has enough unspent amount */
	if (unspent_amount < amount)
	{
		free(transaction);
		return (NULL);
	}
	/* Create the transaction outputs */
	tx_out = send_amount(sender, receiver, amount, unspent_amount);
	if (!tx_out)
	{
		free(transaction);
		return (NULL);
	}
	/* Initialize the transaction */
	transaction->inputs = tx_in, transaction->outputs = tx_out;
	/* Compute the hash of the transaction */
	transaction_hash(transaction, transaction->id);
	args[0] = transaction->id, args[1] = (void *)sender, args[2] = all_unspent;
	/* Sign the transaction inputs */
	llist_for_each(transaction->inputs, sign_transaction_inputs, args);
	return (transaction);
}
