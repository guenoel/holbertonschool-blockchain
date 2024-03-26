#include "transaction.h"

/**
 * tx_in_create - Creates a new transaction input structure
 *
 * @unspent: Pointer to the unspent transaction output to be converted
 *
 * Return: A pointer to the created transaction input upon success, or NULL
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *tx_in = calloc(1, sizeof(*tx_in));

	if (!tx_in)
		return (NULL);
	if (!unspent)
		return (NULL);
	memcpy(tx_in->block_hash, unspent->block_hash, sizeof(tx_in->block_hash));
	memcpy(tx_in->tx_id, unspent->tx_id, sizeof(tx_in->tx_id));
	memcpy(tx_in->tx_out_hash, unspent->out.hash, sizeof(tx_in->tx_out_hash));
	return (tx_in);
}
