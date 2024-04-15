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


#include "../crypto/hblk_crypto.h"
#include "../blockchain/v0.3/blockchain.h"
#include "../blockchain/v0.3/transaction/transaction.h"

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

/* Prototypes Functions */
int handle_exit(void);
int handle_wallet_create(state_t *state);
int handle_wallet_save(state_t *state, char *path);
int handle_wallet_load(state_t *state, char *path);
int handle_send(int amount, char *receiver_address, state_t *state);
uint8_t *pubKeyHexToByteArray(char *address);



#endif /* _CLI_H_ */
