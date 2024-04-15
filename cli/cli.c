#include "cli.h"
#include "../blockchain/v0.3/transaction/transaction.h"

/**
 * handle_exit - handle the exit command
 * Return: 1
 */
int handle_exit(void)
{
	printf("Exiting CLI\n");
	return (1);
}

/**
 * handle_wallet_create - handle the wallet_create command
 * @ec_key: pointer to store EC key
 * Return: 0 if success, -1 if failed
 */
int handle_wallet_create(state_t *state)
{
	state->wallet = ec_create();
	if (state->wallet)
	{
		printf("Create wallet (EC key)\n");
		return (0);
	}
	else
	{
		printf("Failed to create EC key\n");
		return (-1);
	}
}

/**
 * handle_wallet_save - handle the wallet_save command
 * @ec_key: EC key
 * @path: path to save the key pair
 * Return: 0 if success, -1 if failed
 */
int handle_wallet_save(state_t *state, char *path)
{
	if (!state->wallet)
	{
		printf("No EC key available\n");
		return (-1);
	}
	if (ec_save(state->wallet, path))
	{
		printf("Save wallet (EC key pair)\n");
		EC_KEY_free(state->wallet);
		return (0);
	}
	else
	{
		printf("Failed to save EC key\n");
		return (-1);
	}
}

/**
 * handle_wallet_load - handle the wallet_load command
 * @ec_key: pointer to store EC key
 * @path: path to load the key pair
 * Return: 0 if success, -1 if failed
 */
int handle_wallet_load(state_t *state, char *path)
{
	/* if (state->wallet != NULL) */
		/* EC_KEY_free(state->wallet); */
	printf("1: %p\n", (void *)state->wallet);
	state->wallet = ec_load(path);
	printf("2: %p\n", (void *)state->wallet);
	if (state->wallet)
	{
		printf("Load wallet (EC key pair)\n");
		/* Print <base64 encoded data> Key pair */
		/* EC_KEY_print_fp(stdout, state->wallet, 0); */
		return (0);
	}
	else
	{
		printf("Failed to load EC key\n");
		if (state->wallet)
			EC_KEY_free(state->wallet);
		return (-1);
	}
}

/**
 * find_command - find the command to execute
 * @cmd: command
 * @arg1: argument 1
 * @arg2: argument 2
 * @ec_key: pointer to store EC key
 * Return: 0 if ok, 1 if exit
 */
int find_command(char *cmd, char *arg1, char *arg2, state_t *state, char *receiver_address)
{
	if (strcmp(cmd, "exit") == 0)
		return (handle_exit());
	else if (strcmp(cmd, "wallet_create") == 0)
		return (handle_wallet_create(state));
	else if (strcmp(cmd, "wallet_save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_save <path>\n");
			return (-1);
		}
		else
			return (handle_wallet_save(state, arg1));
	}
	else if (strcmp(cmd, "wallet_load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_load <path>\n");
			return (-1);
		}
		else
			return (handle_wallet_load(state, arg1));
	}
	else if (strcmp(cmd, "send") == 0)
	{
		if (!arg1 || !arg2)
		{
			printf("Usage: send <amount> <address>\n");
			return (-1);
		}
		else
		{
			int amount = atoi(arg1);
			strncpy(receiver_address, arg2, (EC_PUB_LEN * 2) + 1);
			return (handle_send(amount, receiver_address, state));
		}
	}
	else
	{
		printf("unknown command: \"%s\"\n", cmd);
		return (-1);
	}
}

/**
 * pubKeyHexToByteArray - converts a EC_KEY public key in a lowercase or
 * uppercase hex string to a byte array
 *
 * @address: public key of EC_PUB_LEN bytes, in hex
 *
 * Return: byte array on success, or NULL on failure
 */
uint8_t *pubKeyHexToByteArray(char *address)
{
	uint8_t *pub;
	unsigned int scan;
	size_t i, j, len, upper, case_detected;

	if (!address)
	{
		fprintf(stderr, "pubKeyHexToByteArray: NULL parameter\n");
		return (NULL);
	}
	pub = calloc(EC_PUB_LEN, sizeof(uint8_t));
	if (!pub)
	{
		fprintf(stderr, "pubKeyHexToByteArray: calloc failure\n");
		return (NULL);
	}
	len = strlen(address);
	if (len < (EC_PUB_LEN * 2))
	{
		free(pub);
		return (NULL);
	}

	for (i = 0, case_detected = 0; i < len; i++)
	{
		if (isdigit(address[i]))
			continue;
		else if (address[i] >= 'a' && address[i] <= 'f')
		{
			if (!case_detected)
			{
				upper = 0;
				case_detected = 1;
			}
			if (upper)
			{
				free(pub);
				return (NULL);
			}
		}
		else if (address[i] >= 'A' && address[i] <= 'F')
		{
			if (!case_detected)
			{
				upper = 1;
				case_detected = 1;
			}
			if (!upper)
			{
				free(pub);
				return (NULL);
			}
		}
		else
		{
			free(pub);
			return (NULL);
		}
	}

	for (i = 0, j = 0; i < EC_PUB_LEN; i++, j += 2)
	{
		if (sscanf(address + j, upper ? "%02X" : "%02x", &scan) == -1)
		{
			perror("pubKeyHexToByteArray: sscanf");
			free(pub);
			return (NULL);
		}
		pub[i] = scan;
	}

	return (pub);
}

/**
 * handle_send - handle the send command
 * @amount: amount to send
 * @receiver_address: receiver address
 * @sender_key: sender key
 * Return: 0 if success, -1 if failed
 */
int handle_send(int amount, char *receiver_address, state_t *state)
{
	uint8_t *pub;
	EC_KEY *receiver_key = NULL;
	transaction_t *tx = NULL;
	int i;

	pub = pubKeyHexToByteArray(receiver_address);

	if (!pub)
	{
		printf("Failed to convert receiver address\n");
		return -1;
	}
	for (i = 0; i < (EC_PUB_LEN); i++)
		printf("%02x", pub[i]);
	printf("\n");

	receiver_key = ec_from_pub(pub);
	if (!receiver_key)
	{
		printf("Failed to create receiver key\n");
		free(pub);
		return -1;
	}

	tx = transaction_create(state->wallet, receiver_key, amount, state->blockchain->unspent);
	if (!tx)
	{
		printf("Failed to create transaction\n");
		EC_KEY_free(receiver_key);
		free(pub);
		return -1;
	}

	if ((transaction_is_valid(tx, state->blockchain->unspent)) == 0)
	{
		printf("Invalid transaction\n");
		transaction_destroy(tx);
		EC_KEY_free(receiver_key);
		free(pub);
		return -1;
	}

	if (llist_add_node(state->tx_pool, tx, ADD_NODE_REAR) == -1)
	{
		printf("Failed to add transaction to pool\n");
		transaction_destroy(tx);
		EC_KEY_free(receiver_key);
		free(pub);
		return -1;
	}

	printf("Transaction sent successfully\n");

	EC_KEY_free(receiver_key);
	free(pub);
	return 0;
}

/**
 * cleanup - free allocated memory before exiting
 * @ec_key: EC key pointer
 */
void cleanup(state_t *state)
{
	if (state)
	{
		if (state->wallet)
			EC_KEY_free(state->wallet);
		if (state->blockchain)
			blockchain_destroy(state->blockchain);
		if (state->tx_pool)
			llist_destroy(state->tx_pool, 1, free);
		free(state);
	}
}

/**
 * main - Entry point
 * Return: 0 if ok, 1 if failed
 */
int main(void)
{
	ssize_t read;
	size_t len;
	int ret = 0;
	char *line = NULL, *cmd, *arg1, *arg2, receiver_address[EC_PUB_LEN * 2];
	state_t *state = NULL;
	blockchain_t *blockchain = blockchain_create();
	llist_t *tx_pool = llist_create(MT_SUPPORT_FALSE);

	if (!blockchain)
	{
		fprintf(stderr, "Failed to create blockchain\n");
		return (1);
	}
	if (!tx_pool)
	{
		fprintf(stderr, "Failed to create list transaction pool\n");
		blockchain_destroy(blockchain);
		return (1);
	}
	state = calloc(1, sizeof(state_t));
	if (!state)
	{
		fprintf(stderr, "Failed to allocate memory for state\n");
		blockchain_destroy(blockchain);
		llist_destroy(tx_pool, 1, NULL);
		return (1);
	}
	state->blockchain = blockchain;
	state->tx_pool = tx_pool;
	state->status = 0;
	state->wallet = NULL;

	while (1)
	{
		printf("CLI prompt> ");
		read = getline(&line, &len, stdin);
		if (read == -1)
			break;
		if (read <= 1)
			continue;
		line = strtok(line, "\n");
		cmd = strtok(line, " ");
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		/* remember to try with & for state */
		ret = find_command(cmd, arg1, arg2, state, receiver_address);
		if (ret == 1)
			break;
	}
	if (line)
		free(line);
	cleanup(state);
	return (ret != 0);
}
