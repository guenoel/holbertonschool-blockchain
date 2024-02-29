#include "hblk_crypto.h"

/**
 * ec_load - loads an EC key pair from the disk
 *
 * @folder: path to the folder in which to save the keys
 *
 * Return: If folder is NULL, return NULL.
 * Otherwise, return a pointer to the created EC_KEY structure.
 */
EC_KEY *ec_load(char const *folder)
{
	char path[1024];
	FILE *fp;
	EC_KEY *key;

	if (!folder)
		return (NULL);

	sprintf(path, "%s/%s", folder, PUB_FILENAME);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);
	if (!PEM_read_EC_PUBKEY(fp, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		fclose(fp);
		return (NULL);
	}
	sprintf(path, "%s/%s", folder, PRI_FILENAME);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);
	if (!PEM_read_ECPrivateKey(fp, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		fclose(fp);
		return (NULL);
	}
	fclose(fp);

	return (key);
}
