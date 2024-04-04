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
* send_amount - Send a specified amount from sender to receiver.
*
* @sender: Key of the sender.
* @receiver: Key of the receiver.
* @amount: Amount to send to the receiver.
* @total: Total balance of the sender.
*
* Return: A list of transaction outputs or NULL on failure.
*/
llist_t *send_amount(EC_KEY const *sender, EC_KEY const *receiver,
					int32_t amount, uint32_t total)
{
	llist_t *transaction_outputs = NULL; /* List to store transaction outputs */
	uint32_t remaining_balance = total - amount; /* Calculate remaining balance */
	uint8_t receiver_pub_key[EC_PUB_LEN]; /* Buffer for receiver's public key */
	tx_out_t *output_node = NULL; /* Pointer to a transaction output node */

	/* Create a list to store transaction outputs */
	transaction_outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction_outputs)
		/* Failed to create the list, return NULL */
		return (NULL);

	/* Create a transaction output for the specified amount */
	ec_to_pub(receiver, receiver_pub_key);/* Get the public key of the receiver */
	output_node = tx_out_create(amount, receiver_pub_key); /* Create the output */
	if (!output_node)
	{
		/* Failed to create the output, clean up and return NULL */
		llist_destroy(transaction_outputs, 1, NULL);
		return (NULL);
	}
	/* Add the output to the list */
	llist_add_node(transaction_outputs, output_node, ADD_NODE_REAR);
	/* If there is a remaining balance, create another transaction output */
	if (remaining_balance != 0)
	{
		uint8_t sender_pub_key[EC_PUB_LEN]; /* Buffer for sender's public key */
		/* Get the public key of the sender */
		ec_to_pub(sender, sender_pub_key);
		/* Create a transaction output for the remaining balance */
		output_node = tx_out_create(remaining_balance, sender_pub_key);
		if (!output_node)
		{
			/* Failed to create the output, clean up and return NULL */
			llist_destroy(transaction_outputs, 1, NULL);
			return (NULL);
		}
		/* Add the output to the list */
		llist_add_node(transaction_outputs, output_node, ADD_NODE_REAR);
	}
	return (transaction_outputs);
}


/**
* transaction_create - creates a transaction
* @sender: key of a sender
* @receiver: key of a receiver
* @amount: amount to send to @receiver
* @all_unspent: all unspent transactions, balance of @sender
* Return: a new transaction or NULL on failure
*/
transaction_t *transaction_create(EC_KEY const *sender,
									EC_KEY const *receiver,
									uint32_t amount, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	transaction_t *transaction;
	llist_t *tr_in, *tr_out;
	void *args[3];
	uint32_t unspent_amount = 0;

	if (!sender || !receiver || !all_unspent)
		return (NULL);
	/* Allocate memory for the transaction */
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (NULL);
	/* Create the transaction inputs */
	tr_in = llist_create(MT_SUPPORT_FALSE);
	/* Get the public key of the sender */
	ec_to_pub(sender, pub);
	/* Select unspent transactions */
	args[0] = pub, args[1] = tr_in, args[2] = &unspent_amount;
	llist_for_each(all_unspent, select_unspent_in, args);
	/* Check if the sender has enough unspent amount */
	if (unspent_amount < amount)
	{
		free(transaction);
		return (NULL);
	}
	/* Create the transaction outputs */
	tr_out = send_amount(sender, receiver, amount, unspent_amount);
	if (!tr_out)
	{
		free(transaction);
		return (NULL);
	}
	/* Initialize the transaction */
	transaction->inputs = tr_in, transaction->outputs = tr_out;
	/* Compute the hash of the transaction */
	transaction_hash(transaction, transaction->id);
	args[0] = transaction->id, args[1] = (void *)sender, args[2] = all_unspent;
	/* Sign the transaction inputs */
	llist_for_each(transaction->inputs, sign_transaction_inputs, args);
	return (transaction);
}
