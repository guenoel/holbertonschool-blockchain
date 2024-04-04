#include "transaction.h"

/**
* tx_out_create - Creates a transaction output structure
* @amount: Amount in the transaction output
* @pub: Receiver's public address
*
* Return: A pointer to the allocated transaction output
*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *output = calloc(1, sizeof(*output));

	if (!output)
		return (NULL);

	output->amount = amount;
	memcpy(output->pub, pub, sizeof(output->pub));

	if (!sha256((int8_t const *)output, sizeof(output->amount) +
											sizeof(output->pub), output->hash))
	/* if (!sha256((const int8_t *)output, sizeof(*output), output->hash)) */
	{
		free(output);
		return (NULL);
	}

	return (output);
}

