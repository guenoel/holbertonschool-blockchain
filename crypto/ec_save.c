#include "hblk_crypto.h"

/**
 * save_ec_key - saves an EC key pair on the disk
 *
 * @key: pointer to the EC key pair to be saved
 * @folder: path to the folder in which to save the keys
 * @filename: name of the file in which to save the key
 *
 * Return: If key, folder, or filename is NULL, return 0.
 * Otherwise, return 1.
 */
int save_ec_key(EC_KEY *key, const char *folder, char *filename)
{
	char path[1024];
	FILE *fp = NULL;

	sprintf(path, "%s/%s", folder, filename);

	/* Save private key */
	fp = fopen(path, "w");
	if (!fp)
	{
		perror("Error opening key file for writing");
		return (0);
	}

	if (strcmp(filename, PRI_FILENAME) == 0)
		if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
		{
			perror("Error writing private key to file");
			fclose(fp);
			return (0);
		}
	if (strcmp(filename, PUB_FILENAME) == 0)
		if (!PEM_write_EC_PUBKEY(fp, key))
		{
			perror("Error writing public key to file");
			fclose(fp);
			return (0);
		}

	fclose(fp);

	return (1);
}

/**
 * ec_save - saves an existing EC key pair on the disk
 *
 * @key: pointer to the EC key pair to be saved
 * @folder: path to the folder in which to save the keys
 *
 * Return: If key or folder is NULL, return 0.
 * Otherwise, return 1.
 */
int ec_save(EC_KEY *key, const char *folder)
{
	/* Create the folder if it doesn't exist */
	if (mkdir(folder, 0700) != 0 && errno != EEXIST)
	{
		perror("Error creating folder");
		return (0);
	}

	/* Save private key in a file */
	if (!save_ec_key(key, folder, PRI_FILENAME))
		return (0);
	/* Save public key in a file */
	if (!save_ec_key(key, folder, PUB_FILENAME))
		return (0);

	return (1);
}
