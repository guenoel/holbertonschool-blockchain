#include "transaction.h"

/**
 * tx_out_create - Creates a new transaction output structure
 *
 * @amount:     Amount received
 * @pub:        Receiver's public address
 *
 * Return: A pointer to the created transaction output upon success, or NULL
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_out = calloc(1, sizeof(*tx_out));

	if (!tx_out)
		return (NULL);
	if (pub == NULL)
		return (NULL);
	tx_out->amount = amount;
	memcpy(tx_out->pub, pub, sizeof(tx_out->pub));
	if (!sha256((int8_t const *)&tx_out->amount,
		sizeof(tx_out->amount) + sizeof(tx_out->pub), tx_out->hash))
	{
		free(tx_out);
		return (NULL);
	}
	return (tx_out);
}
