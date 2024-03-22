<h1 align="center">Blockchain - Block mining</h1>

**Concepts**

For this project, we expect you to look at this concept:

* [[EYNTK] Blockchain - Block mining](./Blockchain_Block_mining.md)

### Resources

**Read or watch:**

* [Everything you need to know - Block mining](https://intranet.hbtn.io/concepts)
* [Provided libraries and sources](https://github.com/hs-hq/holbertonschool-blockchain)

### General
* What is the purpose of the Block difficulty
* What is the Proof of Work
* How to check that a given hash matches a given difficulty
* How to mine a Block
* Why Block mining is brute force only
* How to automatically adjust the difficulty

### Requirements
### General

* Allowed editors: `vi, vim, emacs`
* All your files will be compiled on Ubuntu `14.04 LTS`
* Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall -Werror -Wextra, -pedantic -Wno-deprecated-declarations` **and the linker flags** `-lssl` and `-lcrypto`
* All your files should end with a new line
* A `README.md` file, at the root of the folder of the project, is mandatory
* Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/main/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/main/betty-doc.pl)
* Your library `libhblk_crypto.a`, located in the `crypto` folder will be built and linked during correction
* The prototypes of all your functions should be included in your header file called `blockchain/v0.2/blockchain.h`
* All your header files should be include guarded


### More Info

### Prerequisites

* Create the folder `blockchain/v0.2/` and copy the following files in it:

	* blockchain/v0.1/blockchain_create.c
	* blockchain/v0.1/blockchain_deserialize.c
	* blockchain/v0.1/blockchain_destroy.c
	* blockchain/v0.1/blockchain.h
	* cblockchain/v0.1/blockchain_serialize.c
	* blockchain/v0.1/block_create.c
	* blockchain/v0.1/block_destroy.c
	* blockchain/v0.1/block_hash.c
	* blockchain/v0.1/block_is_valid.c
* Modify your code so when a Blockchain is serialized, the version stored is `0.2`

### Additional information

* Unless specified otherwise, you are allowed to use the C standard library
* You are free to use any data structure that suits you as long as their purpose is well defined
* You are free to print any information **on stderr**, this stream will be discarded during correction. Since you are going to use these utility functions in your Blockchain project, you are free to manage errors as you wish.
* At the end of this project, the structure of your repository should look like this (you may have additional files):

```
holbertonschool-blockchain
├── blockchain
│   ├── v0.1/
│   └── v0.2
│       ├── blockchain_create.c
│       ├── blockchain_deserialize.c
│       ├── blockchain_destroy.c
│       ├── blockchain_difficulty.c
│       ├── blockchain.h
│       ├── blockchain_serialize.c
│       ├── block_create.c
│       ├── block_destroy.c
│       ├── block_hash.c
│       ├── block_is_valid.c
│       ├── block_mine.c
│       ├── hash_matches_difficulty.c
│       ├── libhblk_blockchain_v0.2.a
│       ├── Makefile
│       ├── provided
│       │   ├── _blockchain_destroy.c
│       │   ├── _blockchain_print.c
│       │   ├── _endianness.c
│       │   ├── endianness.h
│       │   └── _genesis.c
│       └── test
│           ├── blockchain_create-main.c
│           ├── blockchain_deserialize-main.c
│           ├── blockchain_destroy-main.c
│           ├── blockchain_difficulty-main.c
│           ├── blockchain_serialize-main.c
│           ├── block_create-main.c
│           ├── block_destroy-main.c
│           ├── block_hash-main.c
│           ├── block_is_valid-main.c
│           ├── block_mine-main.c
│           └── hash_matches_difficulty-main.c
├── crypto/
└── README.md
```
### Tasks
### [0. Proof of work]()

Write a function that checks whether a given hash matches a given difficulty

* Prototype: `int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty);`, where:
	* `hash` is the hash to check
	* `difficulty` is the minimum difficulty the hash should match
* Your function must return `1` if the difficulty is respected, or `0` otherwise
* Refer to the concept page attached to this project to understand how the difficulty is represented in a hash

```
alex@~/holbertonschool-blockchain/blockchain/v0.2$ cat test/hash_matches_difficulty-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

/**
 * _print_hex_buffer - Prints a buffer in its hexadecimal form
 *
 * @buf: Pointer to the buffer to be printed
 * @len: Number of bytes from @buf to be printed
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
    size_t i;

    for (i = 0; buf && i < len; i++)
        printf("%02x", buf[i]);

    fflush(NULL);
}

/**
 * test_hash_matches_difficulty - Test the hash_matches_difficulty function
 *
 * @hash:       Tested hash
 * @difficulty: Tested difficulty
 */
void test_hash_matches_difficulty(uint8_t const *hash, uint32_t difficulty)
{
    printf("Hash: ");
    _print_hex_buffer(hash, SHA256_DIGEST_LENGTH);
    printf("\nDifficulty: %u\n", difficulty);
    if (hash_matches_difficulty(hash, difficulty))
        printf("Matches difficulty\n");
    else
        printf("Doesn't match difficulty\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t hash[SHA256_DIGEST_LENGTH];
    uint32_t difficulty;

    /* Test case 1 */
    memset(hash, 0, SHA256_DIGEST_LENGTH);
    hash[31] = 1;
    difficulty = SHA256_DIGEST_LENGTH * 8 - 1;
    test_hash_matches_difficulty(hash, difficulty);

    /* Test case 2 */
    memset(hash, 0xFF, SHA256_DIGEST_LENGTH);
    hash[0] = 0;
    hash[1] = 98;
    difficulty = 9;
    test_hash_matches_difficulty(hash, difficulty);

    /* Test case 3 */
    memset(hash, 0xFF, SHA256_DIGEST_LENGTH);
    hash[0] = 0;
    hash[1] = 98;
    difficulty = 10;
    test_hash_matches_difficulty(hash, difficulty);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.2$ gcc -Wall -Wextra -Werror -pedantic -I. -I../../crypto -o hash_matches_difficulty-test hash_matches_difficulty.c test/hash_matches_difficulty-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.2$ ./hash_matches_difficulty-test
Hash: 0000000000000000000000000000000000000000000000000000000000000001
Difficulty: 255
Matches difficulty
Hash: 0062ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
Difficulty: 9
Matches difficulty
Hash: 0062ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
Difficulty: 10
Doesn't match difficulty
alex@~/holbertonschool-blockchain/blockchain/v0.2$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.2/hash_matches_difficulty.c`

---

### [1. Block is valid v0.2]()

Modify the function `int block_is_valid(block_t const *block, block_t const *prev_block);` so that it checks that a Block’s hash matches its difficulty

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.2/block_is_valid.c`

---

### [2. Block mining]()

Write a function that mines a Block in order to insert it in the Blockchain

* Prototype: `void block_mine(block_t *block);`, where:
	* `block` points to the Block to be mined
* Your function must find a hash for `block` that matches its difficulty

```
alex@~/holbertonschool-blockchain/blockchain/v0.2$ cat test/block_mine-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

/**
 * _print_hex_buffer - Prints a buffer in its hexadecimal form
 *
 * @buf: Pointer to the buffer to be printed
 * @len: Number of bytes from @buf to be printed
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
    size_t i;

    for (i = 0; buf && i < len; i++)
        printf("%02x", buf[i]);
}

static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
    char const *s)
{
    block_t *block;

    block = block_create(prev, (int8_t *)s, (uint32_t)strlen(s));
    block->info.difficulty = 20;

    block_mine(block);

    if (block_is_valid(block, prev) == 0)
    {
        printf("Block mined: [%u] ", block->info.difficulty);
        _print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
        printf("\n");
        llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    }
    else
    {
        fprintf(stderr, "Invalid Block with index: %u\n",
            block->info.index);
    }

    return (block);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;

    blockchain = blockchain_create();
    block = llist_get_head(blockchain->chain);
    block = _add_block(blockchain, block, "Holberton");
    block = _add_block(blockchain, block, "School");
    block = _add_block(blockchain, block, "of");
    block = _add_block(blockchain, block, "Software");
    block = _add_block(blockchain, block, "Engineering");
    block = _add_block(blockchain, block, "972");
    block = _add_block(blockchain, block, "Mission");
    block = _add_block(blockchain, block, "Street");
    block = _add_block(blockchain, block, "San Francisco");
    block = _add_block(blockchain, block, "CA");
    block = _add_block(blockchain, block, "\\o/");

    _blockchain_print_brief(blockchain);

    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.2$ gcc -Wall -Wextra -Werror -pedantic -I. -I../../crypto -o block_mine-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c block_mine.c provided/_genesis.c provided/_blockchain_print.c test/block_mine-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.2$ ./block_mine-test
Block mined: [20] 00000ba0da3789c8f51aa175079fb46d798a5ee8942d36f8b5759e91fb840437
Block mined: [20] 00000337214df0d6ca983b5444f23ffc83e95bcb88d788a7d8f90928b07ef921
Block mined: [20] 00000df02d9df3064d72feabdf45eeaf8ff95c06720f7248e4874c411593c941
Block mined: [20] 00000026caada9f2987d2c78aa90f6854be215bfe7528f36e76a7079f6e92640
Block mined: [20] 00000c688ce313b4fadb46a2892abf4e8e22802721e1fd9383caa2bffa2a74bb
Block mined: [20] 00000957b4d9d2b6615f2be30fb539c35e4ef4b8d9b991d87330f06695c6a7ad
Block mined: [20] 00000e3ba2b41363f43d8e862328d5a6d5117d6bee2f77482c89b0ba1b99ae84
Block mined: [20] 00000dc416eef392d589aff95c74b1d7cef558a745363f7627c38d6ffa4ddbf9
Block mined: [20] 000003e2a58cc6744b37bd2083df6904865d267f61f333be817372a79ddac1e2
Block mined: [20] 0000035970ce1d658ada23f532bdf123094481bcc23ca41cdaef14e2853970f0
Block mined: [20] 00000e00a52ac590eeecadcee316cb1b67223bef01140866d9e412435290a2c8
Blockchain: {
    chain [12]: [
        Block: {
            info: { 0, 0, 1537578000, 0, 0000000000000000000000000000000000000000000000000000000000000000 },
            data: { "Holberton School", 16 },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: { 1, 20, 1538122982, 1077918, c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 },
            data: { "Holberton", 9 },
            hash: 00000ba0da3789c8f51aa175079fb46d798a5ee8942d36f8b5759e91fb840437
        }
        Block: {
            info: { 2, 20, 1538122982, 326037, 00000ba0da3789c8f51aa175079fb46d798a5ee8942d36f8b5759e91fb840437 },
            data: { "School", 6 },
            hash: 00000337214df0d6ca983b5444f23ffc83e95bcb88d788a7d8f90928b07ef921
        }
        Block: {
            info: { 3, 20, 1538122983, 2455561, 00000337214df0d6ca983b5444f23ffc83e95bcb88d788a7d8f90928b07ef921 },
            data: { "of", 2 },
            hash: 00000df02d9df3064d72feabdf45eeaf8ff95c06720f7248e4874c411593c941
        }
        Block: {
            info: { 4, 20, 1538122984, 951964, 00000df02d9df3064d72feabdf45eeaf8ff95c06720f7248e4874c411593c941 },
            data: { "Software", 8 },
            hash: 00000026caada9f2987d2c78aa90f6854be215bfe7528f36e76a7079f6e92640
        }
        Block: {
            info: { 5, 20, 1538122985, 227082, 00000026caada9f2987d2c78aa90f6854be215bfe7528f36e76a7079f6e92640 },
            data: { "Engineering", 11 },
            hash: 00000c688ce313b4fadb46a2892abf4e8e22802721e1fd9383caa2bffa2a74bb
        }
        Block: {
            info: { 6, 20, 1538122985, 658488, 00000c688ce313b4fadb46a2892abf4e8e22802721e1fd9383caa2bffa2a74bb },
            data: { "972", 3 },
            hash: 00000957b4d9d2b6615f2be30fb539c35e4ef4b8d9b991d87330f06695c6a7ad
        }
        Block: {
            info: { 7, 20, 1538122985, 194854, 00000957b4d9d2b6615f2be30fb539c35e4ef4b8d9b991d87330f06695c6a7ad },
            data: { "Mission", 7 },
            hash: 00000e3ba2b41363f43d8e862328d5a6d5117d6bee2f77482c89b0ba1b99ae84
        }
        Block: {
            info: { 8, 20, 1538122985, 2074049, 00000e3ba2b41363f43d8e862328d5a6d5117d6bee2f77482c89b0ba1b99ae84 },
            data: { "Street", 6 },
            hash: 00000dc416eef392d589aff95c74b1d7cef558a745363f7627c38d6ffa4ddbf9
        }
        Block: {
            info: { 9, 20, 1538122986, 1876803, 00000dc416eef392d589aff95c74b1d7cef558a745363f7627c38d6ffa4ddbf9 },
            data: { "San Francisco", 13 },
            hash: 000003e2a58cc6744b37bd2083df6904865d267f61f333be817372a79ddac1e2
        }
        Block: {
            info: { 10, 20, 1538122987, 115757, 000003e2a58cc6744b37bd2083df6904865d267f61f333be817372a79ddac1e2 },
            data: { "CA", 2 },
            hash: 0000035970ce1d658ada23f532bdf123094481bcc23ca41cdaef14e2853970f0
        }
        Block: {
            info: { 11, 20, 1538122988, 1298771, 0000035970ce1d658ada23f532bdf123094481bcc23ca41cdaef14e2853970f0 },
            data: { "\o/", 3 },
            hash: 00000e00a52ac590eeecadcee316cb1b67223bef01140866d9e412435290a2c8
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.2$
```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.2/block_mine.c`

---

### [3. Adjust difficulty]()

Write a function that computes the difficulty to assign to a potential next Block in the Blockchain.

* Prototype: `uint32_t blockchain_difficulty(blockchain_t const *blockchain);`, where:
	* `blockchain` points to the Blockchain to analyze
* Your function must return the difficulty to be assigned to a potential next Block in the Blockchain
* The following macros must be defined in your header file `blockchain.h`
	* **BLOCK_GENERATION_INTERVAL**
		* Value: **1** (Will be changed during correction)
		* Defines how often (in seconds) a Block should be found
	* **DIFFICULTY_ADJUSTMENT_INTERVAL**
		* Value: **5** (Will be changed during correction)
		* Defines how often (in Blocks) the difficulty should be adjusted
* If the latest Block’s index is a multiple of **DIFFICULTY_ADJUSTMENT_INTERVAL**, AND that it is not the Genesis Block, the difficulty must be adjusted. Otherwise, the difficulty of the latest Block in `blockchain` is returned
* Difficulty adjustment:
	* Retrieve the last Block for which an adjustment was made (the Block with index `<blockchain_size> - DIFFICULTY_ADJUSTMENT_INTERVAL`)
	* Compute the expected elapsed time between the two Blocks
	* Compute the actual elapsed time
	* The difficulty must be incremented if the elapsed time is lower than half the expected elapsed time
	* The difficulty must be decremented if the elapsed time is greater than twice the expected elapsed time
	* The difficulty should not change otherwise

```
alex@~/holbertonschool-blockchain/blockchain/v0.2$ cat test/blockchain_difficulty-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
    char const *s)
{
    block_t *block;

    usleep(300000); /* Simulate block mining ... */
    block = block_create(prev, (int8_t *)s, (uint32_t)strlen(s));

    block->info.difficulty = blockchain_difficulty(blockchain);

    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    return (block);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;

    blockchain = blockchain_create();
    block = llist_get_head(blockchain->chain);
    block = _add_block(blockchain, block, "Holberton");
    block = _add_block(blockchain, block, "School");
    block = _add_block(blockchain, block, "of");
    block = _add_block(blockchain, block, "Software");
    block = _add_block(blockchain, block, "Engineering");
    block = _add_block(blockchain, block, "972");
    block = _add_block(blockchain, block, "Mission");
    block = _add_block(blockchain, block, "Street");
    block = _add_block(blockchain, block, "San Francisco");
    block = _add_block(blockchain, block, "CA");
    block = _add_block(blockchain, block, "\\o/");

    _blockchain_print_brief(blockchain);

    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.2$ gcc -Wall -Wextra -Werror -pedantic -I. -I../../crypto -o blockchain_difficulty-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_difficulty.c provided/_genesis.c provided/_blockchain_print.c test/blockchain_difficulty-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.2$ ./blockchain_difficulty-test
Blockchain: {
    chain [12]: [
        Block: {
            info: { 0, 0, 1537578000, 0, 0000000000000000000000000000000000000000000000000000000000000000 },
            data: { "Holberton School", 16 },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: { 1, 0, 1538119640, 0, c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 },
            data: { "Holberton", 9 },
            hash: 584543539b16396e7aa15459800adb1463e4e7e8f8c5c5564df2aa48e77c5389
        }
        Block: {
            info: { 2, 0, 1538119640, 0, 584543539b16396e7aa15459800adb1463e4e7e8f8c5c5564df2aa48e77c5389 },
            data: { "School", 6 },
            hash: 23112ff5bd4f98dc78bdb3dc1299cdebd46a54438b6737bee6a7e5dab3a15a58
        }
        Block: {
            info: { 3, 0, 1538119641, 0, 23112ff5bd4f98dc78bdb3dc1299cdebd46a54438b6737bee6a7e5dab3a15a58 },
            data: { "of", 2 },
            hash: 13d0c3295a106a27007b2053f1b9e331dbbb14eef145f81ed28307023563a167
        }
        Block: {
            info: { 4, 0, 1538119641, 0, 13d0c3295a106a27007b2053f1b9e331dbbb14eef145f81ed28307023563a167 },
            data: { "Software", 8 },
            hash: 8b75c7bbe56e88e3a4f7476fb5a1a0bc7d83b10df9dcbdbfc619a70a717b0930
        }
        Block: {
            info: { 5, 0, 1538119641, 0, 8b75c7bbe56e88e3a4f7476fb5a1a0bc7d83b10df9dcbdbfc619a70a717b0930 },
            data: { "Engineering", 11 },
            hash: 823fb6cb74082e188bc27c57fe777131accbf34e9ce20b38a79ef76e3deab804
        }
        Block: {
            info: { 6, 1, 1538119642, 0, 823fb6cb74082e188bc27c57fe777131accbf34e9ce20b38a79ef76e3deab804 },
            data: { "972", 3 },
            hash: 05c2eb900762b5d215e05694d8f2d1fdaf9899438210a3152e04c4f1eeba9992
        }
        Block: {
            info: { 7, 1, 1538119642, 0, 05c2eb900762b5d215e05694d8f2d1fdaf9899438210a3152e04c4f1eeba9992 },
            data: { "Mission", 7 },
            hash: 9c5667cdec565167090c803e76608af095dc358fe1e05d420b6017c1fd19337c
        }
        Block: {
            info: { 8, 1, 1538119642, 0, 9c5667cdec565167090c803e76608af095dc358fe1e05d420b6017c1fd19337c },
            data: { "Street", 6 },
            hash: 15a8214ea1d5314d639a30b57fd1d5811c1f087907b845fd0d97b37bd847e5f5
        }
        Block: {
            info: { 9, 1, 1538119643, 0, 15a8214ea1d5314d639a30b57fd1d5811c1f087907b845fd0d97b37bd847e5f5 },
            data: { "San Francisco", 13 },
            hash: 05a4e9489146bb2398237846dbc10a16fd38ff095cee3caa8fb423aa2ba893d0
        }
        Block: {
            info: { 10, 1, 1538119643, 0, 05a4e9489146bb2398237846dbc10a16fd38ff095cee3caa8fb423aa2ba893d0 },
            data: { "CA", 2 },
            hash: 8d5781423017af9622c99a3c87a0128c619d700bd17f71520083ffe62edd7990
        }
        Block: {
            info: { 11, 2, 1538119643, 0, 8d5781423017af9622c99a3c87a0128c619d700bd17f71520083ffe62edd7990 },
            data: { "\o/", 3 },
            hash: 9dba22b332a0adb043556847cdf109e6b124260d61ff7d8fed67956e74fe8436
        }
    ]
}

alex@~/holbertonschool-blockchain/blockchain/v0.2$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.2/blockchain_difficulty.c`
