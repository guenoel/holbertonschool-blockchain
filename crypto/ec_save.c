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

    /* Create the folder if it doesn't exist */
    if (mkdir(folder, 0700) != 0 && errno != EEXIST)
	{
        perror("Error creating folder");
        return 0;
    }

    /* Construct the file paths */
    snprintf(private_key_path, sizeof(private_key_path), "%s/%s", folder, PRI_FILENAME);
    snprintf(public_key_path, sizeof(public_key_path), "%s/%s", folder, PUB_FILENAME);

    /* Save private key */
    FILE *private_key_fp = fopen(private_key_path, "w");
    if (!private_key_fp)
	{
        perror("Error opening private key file for writing");
        return 0;
    }

    if (!PEM_write_ECPrivateKey(private_key_fp, key, NULL, NULL, 0, NULL, NULL)) {
        perror("Error writing private key to file");
        fclose(private_key_fp);
        return 0;
    }

    fclose(private_key_fp);

    /* Save public key */
    FILE *public_key_fp = fopen(public_key_path, "w");
    if (!public_key_fp)
	{
        perror("Error opening public key file for writing");
        return 0;
    }

    if (!PEM_write_EC_PUBKEY(public_key_fp, key))
	{
        perror("Error writing public key to file");
        fclose(public_key_fp);
        return 0;
    }

    fclose(public_key_fp);

    return 1;
}