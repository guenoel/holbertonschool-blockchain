#include "hblk_crypto.h"

/**
 * load_ec_key - loads an EC key pair from the disk
 *
 * @key: pointer to the EC key pair to be loaded
 * @folder: path to the folder in which to save the keys
 * @filename: name of the file in which to save the key
 *
 * Return: If key, folder, or filename is NULL, return 0.
 * Otherwise, return 1.
 */
EC_KEY *load_ec_key(EC_KEY *key, char const *folder, char const *filename)
{
	char path[1024];
	FILE *fp;

	sprintf(path, "%s/%s", folder, filename);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);
	fclose(fp);

	if (!PEM_read_EC_PUBKEY(fp, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		fclose(fp);
		return (NULL);
	}
	fclose(fp);

	/**
	*	if (!PEM_read_ECPrivateKey(fp, &key, NULL, NULL))
	*{
	*	EC_KEY_free(key);
	*	fclose(fp);
	*	return (0);
	*}
	*/

	return (key);
}

EC_KEY *ec_load(char const *folder)
{
	EC_KEY *key;

	if (!folder)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	return (load_ec_key(key, folder, PUB_FILENAME));
}
