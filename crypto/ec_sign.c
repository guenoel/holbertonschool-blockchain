#include "hblk_crypto.h"

/**
 * ec_sign - signs a given set of bytes, using a given EC_KEY private key
 *
 * @key: pointer to the EC_KEY structure containing the private key to be used
 * @msg: characters to be signed
 * @msglen: number of characters to be signed
 * @sig: address at which to store the signature
 *
 * Return: If key, msg, or sig is NULL, or upon failure, return NULL.
 * Otherwise, return a pointer to the signature.
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
					sig_t *sig)
{
	uint32_t len = 0;

	if (!key || !msg || !msglen)
		return (NULL);
	memset(sig->sig, 0, sizeof(sig->sig));
	if (ECDSA_sign(0, msg, msglen, sig->sig, &len, (EC_KEY *)key) != 1)
		return (NULL);
	sig->len = (uint8_t)len;
	return (sig->sig);
}
