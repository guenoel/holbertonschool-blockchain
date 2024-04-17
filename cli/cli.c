#include "cli.h"

/**
* find_command - find the command to execute
* @cmd: command
* @arg1: argument 1
* @arg2: argument 2
* @state: pointer to store EC key
* @receiver_address: receiver address
* Return: 0 if ok, 1 if exit
*/
int find_command(char *cmd, char *arg1, char *arg2, state_t *state,
					char *receiver_address)
{
	if (strcmp(cmd, "exit") == 0)
		return (handle_exit());
	else if (strcmp(cmd, "wallet_create") == 0)
	{
		if (arg1)
		{
			printf("Usage: wallet_create\n");
			printf("Too many arguments\n");
			return (-1);
		}
		else
			return (handle_wallet_create(state));
	}
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
	else if (strcmp(cmd, "mine") == 0)
	{
		if (arg1)
		{
			printf("Usage: mine\n");
			printf("Too many arguments\n");
			return (-1);
		}
		else
			return (handle_mine(state));
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
	else if (strcmp(cmd, "info") == 0)
		return (handle_info(state));
	else if (strcmp(cmd, "load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: load <path>\n");
			return (-1);
		}
		else
			return (handle_load(state, arg1));
	}
	else if (strcmp(cmd, "save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: save <path>/<file>\n");
			return (-1);
		}
		else
			return (handle_save(state, arg1));
	}
	else
	{
		printf("unknown command: \"%s\"\n", cmd);
		return (-1);
	}
}

void print_logo(void)
{
	char *filename = "./logo.txt";
	FILE *f = fopen(filename, "r");
	char buffer[256];

	if (f == NULL)
	{
		printf("Error: could not open file %s", filename);
	}

	while (fgets(buffer, 256, f))
		printf("%s", buffer);

	fclose(f);
}

/**
* main - Entry point
* Return: 0 if ok, 1 if failed
*/
int main(void)
{
	int ret = 0;
	char *line = NULL, *cmd, *arg1, *arg2, receiver_address[EC_PUB_LEN * 2];
	state_t *state = NULL;
	blockchain_t *blockchain = blockchain_create();
	llist_t *tx_pool = llist_create(MT_SUPPORT_FALSE);

	print_logo();
	if (!blockchain)
	{
		fprintf(stderr, "Error: Fallo al crear la cadena de bloques\n");
		return (1);
	}
	if (!tx_pool)
	{
		fprintf(stderr, "Error: Fallo al crear la lista de transacciones\n");
		blockchain_destroy(blockchain);
		return (1);
	}
	state = calloc(1, sizeof(state_t));
	if (!state)
	{
		fprintf(stderr, "Error: Fallo al asignar memoria para el estado\n");
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
		line = readline("Holbie blockchain> ");
		if (!line)
			break;

		if (line[0] != '\0')
		{
			add_history(line);
			cmd = strtok(line, " ");
			arg1 = strtok(NULL, " ");
			arg2 = strtok(NULL, " ");

			ret = find_command(cmd, arg1, arg2, state, receiver_address);
			if (ret == 1)
				break;
		}

		free(line);
	}

	cleanup(state);
	return (ret != 0);
}

