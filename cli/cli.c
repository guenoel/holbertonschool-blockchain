#include "cli.h"

/**
 * find_command - find the command to execute
 * @cmd: command
 * @arg1: argument 1
 * @arg2: argument 2
 * @ec_key: EC key
 * Return: 0 if ok, 1 if exit
*/
int find_command(char *cmd, char *arg1, char *arg2, EC_KEY **ec_key)
{
	if (strcmp(cmd, "exit") == 0)
	{
		return (1);
	}
	else if (strcmp(cmd, "wallet_create") == 0)
	{
		*ec_key = ec_create();
		if (*ec_key)
			printf("Create wallet (EC key)\n");
		else
			printf("Failed to create EC key\n");
	}
	else if (strcmp(cmd, "wallet_save") == 0)
	{
		if (!arg1)
			printf("Usage: wallet_save <path>\n");
		else
		{
			if (!*ec_key)
				printf("No ec_key variable\n");
			else
			{
				ec_save(*ec_key, arg1);
				printf("Save wallet (EC key pair)\n");
			}
		}
	}
	else if (strcmp(cmd, "wallet_load") == 0)
	{
		if (!arg1)
			printf("Usage: wallet_load <path>\n");
		else
		{
			*ec_key = ec_load(arg1);
			if (*ec_key)
				printf("Load wallet (EC key pair)\n");
			else
				printf("Failed to load EC key\n");
		}
	}
	else
		printf("unknown command: \"%s\"\n", cmd);
	return (0);
	(void)arg2;
}

/**
 * main - Entry point
 * Return: 0 if ok, 1 if failed
*/
int main(void)
{
	EC_KEY *ec_key = NULL;
	ssize_t read;
	size_t len;
	int ret = 0;
	char *line = NULL, *cmd, *arg1, *arg2;

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

		ret = find_command(cmd, arg1, arg2, &ec_key);
		if (ret == 1)
			break;
	}
	if (line)
		free(line);
	return (0);
}
