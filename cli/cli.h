#ifndef _CLI_H_
#define _CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
/* #include <sys/wait.h> */


#include "../crypto/hblk_crypto.h"
#include "../blockchain/v0.3/blockchain.h"
#include "../blockchain/v0.3/transaction/transaction.h"

#include <readline/readline.h>
#include <readline/history.h>

#define ADDRESS_MAX_LEN 42


/**
* struct state_s - client state structure
*
* @status: most recent executed command status
* @wallet: ec_keys
* @blockchain: blockchain
* @tx_pool: transaction pool
*/
typedef struct state_s
{
	int status;
	EC_KEY *wallet;
	blockchain_t *blockchain;
	llist_t *tx_pool;
} state_t;

/**
* struct user_s - user structure
* @username: username
* @wallet: wallet
*/
typedef struct user_s
{
	char *username;
	EC_KEY *wallet;
} user_t;


/* Prototypes Functions */
int handle_exit(void);
int handle_wallet_create(state_t *state);
int handle_wallet_save(state_t *state, char *path);
int handle_wallet_load(state_t *state, char *path);
int handle_send(int amount, char *receiver_address, state_t *state);
int handle_info(state_t *state);
int handle_load(state_t *state, char *path);
int handle_save(state_t *state, char *path);
int handle_mine(state_t *state);
int invalid_tx(llist_node_t node, void *arg);
int add_transaction(llist_node_t node, unsigned int idx, void *arg);
int invalid_tx(llist_node_t node, void *arg);
int sum_unspent(void *node, unsigned int idx, void *arg);
int mine(state_t *state, block_t *block, block_t *prev_block,
		transaction_t *coinbase_tx);
void cleanup(state_t *state);
uint8_t *string_to_pub(char *address);
int str_to_key(char *str, EC_KEY **key);
int find_command(char *cmd, char *arg1, char *arg2, state_t *state,
					char *receiver_address);

#endif /* _CLI_H_ */
