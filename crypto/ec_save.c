#include "hblk_crypto.h"

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
	char private_key_path[1024];
	char public_key_path[1024];
	FILE *private_key_fp = NULL;
	FILE *public_key_fp = NULL;

	/* Create the folder if it doesn't exist */
	if (mkdir(folder, 0700) != 0 && errno != EEXIST)
	{
		perror("Error creating folder");
		return (0);
	}

	/* Construct the file paths */
	sprintf(private_key_path, "%s/%s",
				folder, PRI_FILENAME);
	sprintf(public_key_path, "%s/%s",
				folder, PUB_FILENAME);

	/* Save private key */
	private_key_fp = fopen(private_key_path, "w");
	if (!private_key_fp)
	{
		perror("Error opening private key file for writing");
		return (0);
	}

	if (!PEM_write_ECPrivateKey(private_key_fp, key, NULL, NULL, 0, NULL, NULL))
	{
		perror("Error writing private key to file");
		fclose(private_key_fp);
		return (0);
	}

	fclose(private_key_fp);

	/* Save public key */
	public_key_fp = fopen(public_key_path, "w");
	if (!public_key_fp)
	{
		perror("Error opening public key file for writing");
		return (0);
	}

	if (!PEM_write_EC_PUBKEY(public_key_fp, key))
	{
		perror("Error writing public key to file");
		fclose(public_key_fp);
		return (0);
	}

	fclose(public_key_fp);

	return (1);
}
