#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts the public key from an EC_KEY struct
 *
 * @key: pointer to the EC_KEY struct to retrieve the public key from
 * @pub: address at which to store the extracted public key
 *
 * Return: If key is NULL or pub is NULL, return NULL.
 * Otherwise, return a pointer to the extracted public key.
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	if (!key || !pub)
		return (NULL);

	/* Extract the public key from the EC_KEY struct */
	if (!EC_POINT_point2oct(EC_KEY_get0_group(key),
		EC_KEY_get0_public_key(key), POINT_CONVERSION_UNCOMPRESSED,
		pub, EC_PUB_LEN, NULL))
		return (NULL);

	return (pub);
}
