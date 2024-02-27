#include "hblk_crypto.h"

/**
 * sha256 - Computes the hash of a sequence of bytes
 *
 * @s: Pointer to the sequence of bytes to be hashed
 * @len: Number of bytes from @s to be hashed
 * @digest: Resulting hash
 *
 * Return: If @s is NULL, @len is 0, or @digest is NULL, return NULL.
 * Otherwise, return @digest.
 */
uint8_t *sha256(int8_t const *s, size_t len,
					uint8_t digest[SHA256_DIGEST_LENGTH])
{
	/* declaration of variables */
	SHA256_CTX contex;

	if (!s || !digest)
		return (NULL);

	/* initialize contex variable */
	if (!SHA256_Init(&contex))
		return (NULL);

	if (SHA256_Update(&contex, s, len) != 1)
		return (NULL);

	if (!SHA256_Final(digest, &contex))
		return (NULL);

	return (digest);
}
