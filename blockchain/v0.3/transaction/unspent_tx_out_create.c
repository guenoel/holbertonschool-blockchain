#include "transaction.h"

/**
 * unspent_tx_out_create - Creates an unspent transaction output structure
 *
 * @block_hash: Hash of the Block containing the transaction @tx_id
 * @tx_id:      ID of the transaction containing @tx_out_hash
 * @out:        Copy of the referenced transaction output
 *
 * Return: A pointer to the created unspent transaction output upon success,
 * or NULL
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *unspent_tx_out = calloc(1, sizeof(*unspent_tx_out));

	if (!unspent_tx_out)
		return (NULL);
	memcpy(unspent_tx_out->block_hash, block_hash,
			sizeof(unspent_tx_out->block_hash));
	memcpy(unspent_tx_out->tx_id, tx_id, sizeof(unspent_tx_out->tx_id));
	memcpy(&unspent_tx_out->out, out, sizeof(unspent_tx_out->out));
	return (unspent_tx_out);
}
