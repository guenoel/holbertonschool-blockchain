#include "hblk_crypto.h"

/**
 * ec_verify - verifies the signature of a given set of bytes, using a given
 * EC_KEY public key
 *
 * @key: pointer to the EC_KEY structure containing the public key to be used
 * @msg: characters to be verified
 * @msglen: number of characters to be verified
 * @sig: signature to be checked
 *
 * Return: If key, msg, or sig is NULL, or upon failure, return 0.
 * Otherwise, return 1 if the signature is valid, and 0 if it is not.
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sig_t const *sig)
{
	if (!key || !msg || !msglen || !sig)
		return (0);
	return (ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key)
			== 1);
}
