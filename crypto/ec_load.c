#include "hblk_crypto.h"

/**
 * ec_load - loads an EC key pair from the disk
 *
 * @folder: path to the folder from which to load the keys
 *
 * Return: If folder is NULL, or upon failure, return NULL.
 * Otherwise, return a pointer to the loaded EC key pair.
 */
EC_KEY *ec_load(char const *folder)
{
	EC_KEY *key;
	char path[1024];
	FILE *fp;

	if (!folder)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	sprintf(path, "%s/%s", folder, PUB_FILENAME);
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

	return (key);
}
