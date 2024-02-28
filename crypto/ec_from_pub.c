#include "hblk_crypto.h"

/**
 * ec_from_pub - creates an EC_KEY structure given a public key
 *
 * @pub: address from which to retrieve the public key
 *
 * Return: If pub is NULL or invalid, return NULL.
 * Otherwise, return a pointer to the created EC_KEY structure.
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key; /* EC_GROUP group, EC_POINT pubkey, BIGNUM privkey, ..others */
	EC_POINT *point; /* context, X, Y (sometime Z) */
	const EC_GROUP *group; /* method, generator, order, cofactor, meth_data */

	if (!pub)
		return (NULL);

	/* Create a new empty EC_KEY structure */
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);
	/* Get the group of the EC_KEY structure */
	group = EC_KEY_get0_group(key);
	/* Create a new empty EC_POINT structure */
	point = EC_POINT_new(group);
	if (!point)
	{
		EC_KEY_free(key);
		return (NULL);
	}
	/* Convert the public key octet string to an EC_POINT */
	if (!EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL))
	{
		EC_KEY_free(key);
		EC_POINT_free(point);
		return (NULL);
	}
	/* Set the public key of the EC_KEY structure */
	if (!EC_KEY_set_public_key(key, point))
	{
		EC_KEY_free(key);
		EC_POINT_free(point);
		return (NULL);
	}
	EC_POINT_free(point); /* Free the EC_POINT structure */

	return (key);
}
