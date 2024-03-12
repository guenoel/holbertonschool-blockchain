<h1 align="center">Blockchain - Data structures</h1>

**Concepts**

For this project, we expect you to look at this concept:

* [[EYNTK] Blockchain - Data structures](./Blockchain_Data_structures.md)

### Resources

**Read or watch:**

* [Everything You Need To Know - Concept page](https://intranet.hbtn.io/concepts)
* [Provided libraries and sources](https://github.com/hs-hq/holbertonschool-blockchain)

### General
* How a Blockchain is structured
* How Blocks refer to each other
* How immutability is maintained in a Blockchain
* What is the **Genesis Block**, and why is it important
* How to hash a Block, and what information is stored in the hash

### Requirements
### General

* Allowed editors: `vi, vim, emacs`
* All your files will be compiled on Ubuntu `14.04 LTS`
* Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall -Werror -Wextra, -pedantic -Wno-deprecated-declarations` **and the linker flags** `-lssl` and `-lcrypto`
* All your files should end with a new line
* A `README.md` file, at the root of the folder of the project, is mandatory
* Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/main/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/main/betty-doc.pl)
* Your library `libhblk_crypto.a`, located in the `crypto` folder will be built and linked during correction
* The prototypes of all your functions should be included in your header file called `blockchain/v0.1/blockchain.h`
* All your header files should be include guarded

### Additional information

* Unless specified otherwise, you are allowed to use the C standard library
* You are free to use any data structure that suits you as long as their purpose is well defined
* You are free to print any information **on stderr**, this stream will be discarded during correction. Since you are going to use these utility functions in your Blockchain project, you are free to manage errors as you wish.
* At the end of this project, the structure of your repository should look like this (you may have additional files):

```
holbertonschool-blockchain
├── blockchain
│   └── v0.1
│       ├── blockchain_create.c
│       ├── blockchain_deserialize.c
│       ├── blockchain_destroy.c
│       ├── blockchain.h
│       ├── blockchain_serialize.c
│       ├── block_create.c
│       ├── block_destroy.c
│       ├── block_hash.c
│       ├── block_is_valid.c
│       ├── libhblk_blockchain_v0.1.a
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
│           ├── blockchain_serialize-main.c
│           ├── block_create-main.c
│           ├── block_destroy-main.c
│           ├── block_hash-main.c
│           └── block_is_valid-main.c
├── crypto/
└── README.md
```

### Tasks
### [0. Create Blockchain]()

Write a function that creates a `Blockchain` structure, and initializes it.

* Prototype: `blockchain_t *blockchain_create(void);`
* The Blockchain must contain one block upon creation. This block is called the **Genesis** Block. Its content is static, and pre-defined as follows:
    * index -> `0`
    * difficulty -> `0`
    * timestamp -> `1537578000`
    * nonce -> `0`
    * prev_hash -> `0` (times 32)
    * data -> `"Holberton School"` (16 bytes long)
    * hash -> `c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803`
    * Note: The above is the hexadecimal representation of the `hash`, you can store it in the hash variable as follows:
```
hash = "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" /* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */
```

Example:

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/blockchain_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <llist.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    blockchain_t *blockchain2;

    blockchain = blockchain_create();

    _blockchain_print(blockchain);

    blockchain2 = blockchain_create();
    if (memcmp(llist_get_head(blockchain->chain),
        llist_get_head(blockchain2->chain), sizeof(block_t)) != 0)
    {
        fprintf(stderr, "Genesis Block should always be the same\n");
        _blockchain_destroy(blockchain);
        _blockchain_destroy(blockchain2);
        return (EXIT_FAILURE);
    }

    _blockchain_destroy(blockchain);
    _blockchain_destroy(blockchain2);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o blockchain_create-test blockchain_create.c test/blockchain_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./blockchain_create-test
Blockchain: {
    chain [1]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```

### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/blockchain_create.c`

---

### [1. Create Block]()

Write a function that creates a Block structure and initializes it.

* Prototype: `block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)`, where:
    * `prev` is a pointer to the previous Block in the Blockchain
    * `data` points to a memory area to duplicate in the Block’s `data`
    * `data_len` stores the number of bytes to duplicate in data. If data_len is bigger than `BLOCKCHAIN_DATA_MAX`, then only `BLOCKCHAIN_DATA_MAX` bytes must be duplicated.
    * The new Block’s index must be the previous Block’s index + 1
    * The new Block’s `difficulty` and `nonce` must both be initialized to `0`
    * The new Block’s `timestamp` should be initialized using the `time(2)` system call. It is normal if your value differs from the following example.
    * The new Block’s `hash` must be zeroed
* Your function must return a pointer to the allocated Block

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

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

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"School", 6);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    _blockchain_print(blockchain);
    _blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_create-test blockchain_create.c block_create.c test/block_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./block_create-test
Blockchain: {
    chain [3]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: {
                index: 1,
                difficulty: 0,
                timestamp: 1536714911,
                nonce: 0,
                prev_hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
            },
            data: {
                buffer: "Holberton",
                len: 9
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
        Block: {
            info: {
                index: 2,
                difficulty: 0,
                timestamp: 1536714911,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "School",
                len: 6
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.1/block_create.c`

---

### [2. Delete Block]()

Write a function that deletes an existing Block

* Prototype: `void block_destroy(block_t *block)`;, where:
    * `block` points to the Block to delete

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_destroy-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_destroy(blockchain_t *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *first, *block1, *block2;

    blockchain = blockchain_create();
    first = llist_pop(blockchain->chain);
    block1 = block_create(first, (int8_t *)"Holberton", 9);
    block2 = block_create(block1, (int8_t *)"School", 6);

    block_destroy(first);
    block_destroy(block1);
    block_destroy(block2);

    _blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_destroy-test blockchain_create.c block_create.c block_destroy.c test/block_destroy-main.c provided/_blockchain_destroy.c  -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ valgrind ./block_destroy-test
==3393== Memcheck, a memory error detector
==3393== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==3393== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==3393== Command: ./block_destroy-test
==3393==
==3393==
==3393== HEAP SUMMARY:
==3393==     in use at exit: 0 bytes in 0 blocks
==3393==   total heap usage: 6 allocs, 6 frees, 3,480 bytes allocated
==3393==
==3393== All heap blocks were freed -- no leaks are possible
==3393==
==3393== For counts of detected and suppressed errors, rerun with: -v
==3393== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```

### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/block_destroy.c`

---

### [3. Delete Blockchain]()

Write a function that deletes an existing Blockchain, along with all the Blocks it contains

* Prototype: `void blockchain_destroy(blockchain_t *blockchain)`;, where:
    * `blockchain` points to the Blockchain structure to delete
* All the Blocks in the `chain` linked list must be destroyed

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/blockchain_destroy-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

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

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"School", 6);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"of", 2);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"Software", 8);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"Engineering", 11);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o blockchain_destroy-test blockchain_destroy.c blockchain_create.c block_create.c block_destroy.c test/blockchain_destroy-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ valgrind ./blockchain_destroy-test
==3405== Memcheck, a memory error detector
==3405== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==3405== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==3405== Command: ./blockchain_destroy-test
==3405==
==3405==
==3405== HEAP SUMMARY:
==3405==     in use at exit: 0 bytes in 0 blocks
==3405==   total heap usage: 14 allocs, 14 frees, 6,920 bytes allocated
==3405==
==3405== All heap blocks were freed -- no leaks are possible
==3405==
==3405== For counts of detected and suppressed errors, rerun with: -v
==3405== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```

### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/blockchain_destroy.c`

### [4. Hash Block]()

Write a function that computes the hash of a Block

* Prototype: `uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);`, where:
    * `block` points to the Block to be hashed
* The resulting hash must be stored in `hash_buf`
* `block->hash` must be left unchanged.
* Your function must return a pointer to `hash_buf`
* You don’t have to allocate any memory dynamically

**Note:** With this first iteration of our Block structure, the only data that is unpredictable is the timestamp. Feel free to hardcode the timestamp from the following example to validate your output.

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_hash-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

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

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    _blockchain_print(blockchain);

    block_hash(block, block->hash);
    block = block_create(block, (int8_t *)"School", 6);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    _blockchain_print(blockchain);

    _blockchain_destroy(blockchain);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_hash-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c test/block_hash-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./block_hash-test
Blockchain: {
    chain [2]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: {
                index: 1,
                difficulty: 0,
                timestamp: 1536715352,
                nonce: 0,
                prev_hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
            },
            data: {
                buffer: "Holberton",
                len: 9
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
    ]
}
Blockchain: {
    chain [3]: [
        Block: {
            info: {
                index: 0,
                difficulty: 0,
                timestamp: 1537578000,
                nonce: 0,
                prev_hash: 0000000000000000000000000000000000000000000000000000000000000000
            },
            data: {
                buffer: "Holberton School",
                len: 16
            },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: {
                index: 1,
                difficulty: 0,
                timestamp: 1536715352,
                nonce: 0,
                prev_hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
            },
            data: {
                buffer: "Holberton",
                len: 9
            },
            hash: ddcfaf37a7509f99f698bc055885b222e690efe26a74a71eed452dc0f0808d46
        }
        Block: {
            info: {
                index: 2,
                difficulty: 0,
                timestamp: 1536715352,
                nonce: 0,
                prev_hash: ddcfaf37a7509f99f698bc055885b222e690efe26a74a71eed452dc0f0808d46
            },
            data: {
                buffer: "School",
                len: 6
            },
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```

### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/block_hash.c`

---

### [5. Save Blockchain]()

Write a function that serializes a Blockchain into a file

* Prototype: `int blockchain_serialize(blockchain_t const *blockchain, char const *path);`, where:
    * `blockchain` points to the Blockchain to be serialized,
    * `path` contains the path to a file to serialize the Blockchain into
* If `path` points to an existing file, it must be overwritten
*   Your function must return `0` upon success, or `-1` upon failure

**Format**
The resulting file should contain exactly the following:

**File Header**

The following table describes the file header

**Offset**	**Size (bytes)**	**Field**	**Purpose**

0x00	4	hblk_magic	Identifies the file as a valid serialized Blockchain format
HBLK (48 42 4c 4b) in ASCII; these four bytes constitute the magic number

0x04	3	hblk_version	Identifies the version at which the Blockchain has been serialized
The format is X.Y, where both X and Y are ASCII characters between 0 and 9

0x07	1	hblk_endian	This byte is set to either 1 or 2 to signify little or big endianness, respectively
This affects interpretation of multi-byte fields

0x08	4	hblk_blocks	Number of blocks in the Blockchain
Endianness dependent

0x0C	?	blocks	List of Blocks
Block

The following table describes how a block is serialized. The Blocks are serialized contiguously, the first one starting at offset 0x0C

**Offset**	**Size (bytes)**	**Field**	**Purpose**

0x00	4	index	Block index in the Blockchain
Endianness dependent

0x04	4	difficulty	Block difficulty
Endianness dependent

0x08	8	timestamp	Time at which the block was created (UNIX timestamp)
Endianness dependent

0x10	8	nonce	Salt value used to alter the Block hash
Endianness dependent

0x18	32	prev_hash	Hash of the previous Block in the Blockchain
0x38	4	data_len	Block data size (in bytes)
Endianness dependent

0x3C	data_len	data	Block data
0x3C + data_len	32	hash	Block hash

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/blockchain_serialize-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

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

    block = block_create(block, (int8_t *)"Holberton", 9);
    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"School", 6);
    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"of", 2);
    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"Software", 8);
    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    block = block_create(block, (int8_t *)"Engineering", 11);
    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    blockchain_serialize(blockchain, "save.hblk");

    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o blockchain_serialize-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_serialize.c provided/_endianness.c test/blockchain_serialize-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./blockchain_serialize-test
alex@~/holbertonschool-blockchain/blockchain/v0.1$ hexdump -Cv save.hblk
00000000  48 42 4c 4b 30 2e 31 01  06 00 00 00 00 00 00 00  |HBLK0.1.........|
00000010  00 00 00 00 10 94 a5 5b  00 00 00 00 00 00 00 00  |.......[........|
00000020  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000030  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000040  00 00 00 00 10 00 00 00  48 6f 6c 62 65 72 74 6f  |........Holberto|
00000050  6e 20 53 63 68 6f 6f 6c  c5 2c 26 c8 b5 46 16 39  |n School.,&..F.9|
00000060  63 5d 8e df 2a 97 d4 8d  0c 8e 00 09 c8 17 f2 b1  |c]..*...........|
00000070  d3 d7 ff 2f 04 51 58 03  01 00 00 00 00 00 00 00  |.../.QX.........|
00000080  b5 ec 9f 5b 00 00 00 00  00 00 00 00 00 00 00 00  |...[............|
00000090  c5 2c 26 c8 b5 46 16 39  63 5d 8e df 2a 97 d4 8d  |.,&..F.9c]..*...|
000000a0  0c 8e 00 09 c8 17 f2 b1  d3 d7 ff 2f 04 51 58 03  |.........../.QX.|
000000b0  09 00 00 00 48 6f 6c 62  65 72 74 6f 6e 78 d9 89  |....Holbertonx..|
000000c0  7a 56 31 e0 84 11 01 a2  91 e8 04 0f d5 25 cd 72  |zV1..........%.r|
000000d0  dd bf 64 97 33 10 4b dd  0a 04 59 e5 cf 02 00 00  |..d.3.K...Y.....|
000000e0  00 00 00 00 00 b5 ec 9f  5b 00 00 00 00 00 00 00  |........[.......|
000000f0  00 00 00 00 00 78 d9 89  7a 56 31 e0 84 11 01 a2  |.....x..zV1.....|
00000100  91 e8 04 0f d5 25 cd 72  dd bf 64 97 33 10 4b dd  |.....%.r..d.3.K.|
00000110  0a 04 59 e5 cf 06 00 00  00 53 63 68 6f 6f 6c d3  |..Y......School.|
00000120  83 72 86 9e 97 33 b0 a7  10 f9 2b 55 6f 65 6d ec  |.r...3....+Uoem.|
00000130  b2 98 97 3a 5f 09 52 b0  15 42 72 6f ca 61 56 03  |...:_.R..Bro.aV.|
00000140  00 00 00 00 00 00 00 b5  ec 9f 5b 00 00 00 00 00  |..........[.....|
00000150  00 00 00 00 00 00 00 d3  83 72 86 9e 97 33 b0 a7  |.........r...3..|
00000160  10 f9 2b 55 6f 65 6d ec  b2 98 97 3a 5f 09 52 b0  |..+Uoem....:_.R.|
00000170  15 42 72 6f ca 61 56 02  00 00 00 6f 66 8f 9c 67  |.Bro.aV....of..g|
00000180  71 a1 2e 28 a6 73 a6 7c  93 c3 c3 fd 08 5a 51 b2  |q..(.s.|.....ZQ.|
00000190  3a 59 d7 fd 2d 9f 1a a2  c0 a9 ce da cc 04 00 00  |:Y..-...........|
000001a0  00 00 00 00 00 b5 ec 9f  5b 00 00 00 00 00 00 00  |........[.......|
000001b0  00 00 00 00 00 8f 9c 67  71 a1 2e 28 a6 73 a6 7c  |.......gq..(.s.||
000001c0  93 c3 c3 fd 08 5a 51 b2  3a 59 d7 fd 2d 9f 1a a2  |.....ZQ.:Y..-...|
000001d0  c0 a9 ce da cc 08 00 00  00 53 6f 66 74 77 61 72  |.........Softwar|
000001e0  65 7d cd 9c fd 6f a5 93  6b f1 dc 68 11 17 53 21  |e}...o..k..h..S!|
000001f0  dd 39 33 24 67 91 a6 89  0a 48 d1 76 ee af 41 20  |.93$g....H.v..A |
00000200  38 05 00 00 00 00 00 00  00 b5 ec 9f 5b 00 00 00  |8...........[...|
00000210  00 00 00 00 00 00 00 00  00 7d cd 9c fd 6f a5 93  |.........}...o..|
00000220  6b f1 dc 68 11 17 53 21  dd 39 33 24 67 91 a6 89  |k..h..S!.93$g...|
00000230  0a 48 d1 76 ee af 41 20  38 0b 00 00 00 45 6e 67  |.H.v..A 8....Eng|
00000240  69 6e 65 65 72 69 6e 67  41 d1 b1 78 69 0e 0b 03  |ineeringA..xi...|
00000250  2e b8 56 ed 58 dd 78 70  99 df bb cf 42 db 6a 4f  |..V.X.xp....B.jO|
00000260  13 cb 89 37 42 7e b7 d2                           |...7B~..|
00000268
alex@~/holbertonschool-blockchain/blockchain/v0.1$ 
```

### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/blockchain_serialize.c`

---

### [6. Load Blockchain]()

Write a function that deserializes a Blockchain from a file

* Prototype: `blockchain_t *blockchain_deserialize(char const *path);`, where:
    * `path` contains the path to a file to load the Blockchain from
* If the file pointed to by pathdoesn’t exist, or you don’t have the permission to open/read it, your function must fail
* Your function must return a pointer to the deserialized Blockchain upon success, or `NULL` upon failure
* The format of the file to parse is described in the previous task
* Upon any error, the function must fail and return `NULL`. Error cases:
    * Unable to open/read file
    * Incorrect magic number
    * Version mismatch

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/blockchain_deserialize-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;

    blockchain = blockchain_deserialize("save.hblk");

    _blockchain_print_brief(blockchain);
    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o blockchain_deserialize-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_deserialize.c provided/_blockchain_print.c provided/_endianness.c test/blockchain_deserialize-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ hexdump -Cv save.hblk
00000000  48 42 4c 4b 30 2e 31 01  06 00 00 00 00 00 00 00  |HBLK0.1.........|
00000010  00 00 00 00 10 94 a5 5b  00 00 00 00 00 00 00 00  |.......[........|
00000020  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000030  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000040  00 00 00 00 10 00 00 00  48 6f 6c 62 65 72 74 6f  |........Holberto|
00000050  6e 20 53 63 68 6f 6f 6c  c5 2c 26 c8 b5 46 16 39  |n School.,&..F.9|
00000060  63 5d 8e df 2a 97 d4 8d  0c 8e 00 09 c8 17 f2 b1  |c]..*...........|
00000070  d3 d7 ff 2f 04 51 58 03  01 00 00 00 00 00 00 00  |.../.QX.........|
00000080  b5 ec 9f 5b 00 00 00 00  00 00 00 00 00 00 00 00  |...[............|
00000090  c5 2c 26 c8 b5 46 16 39  63 5d 8e df 2a 97 d4 8d  |.,&..F.9c]..*...|
000000a0  0c 8e 00 09 c8 17 f2 b1  d3 d7 ff 2f 04 51 58 03  |.........../.QX.|
000000b0  09 00 00 00 48 6f 6c 62  65 72 74 6f 6e 78 d9 89  |....Holbertonx..|
000000c0  7a 56 31 e0 84 11 01 a2  91 e8 04 0f d5 25 cd 72  |zV1..........%.r|
000000d0  dd bf 64 97 33 10 4b dd  0a 04 59 e5 cf 02 00 00  |..d.3.K...Y.....|
000000e0  00 00 00 00 00 b5 ec 9f  5b 00 00 00 00 00 00 00  |........[.......|
000000f0  00 00 00 00 00 78 d9 89  7a 56 31 e0 84 11 01 a2  |.....x..zV1.....|
00000100  91 e8 04 0f d5 25 cd 72  dd bf 64 97 33 10 4b dd  |.....%.r..d.3.K.|
00000110  0a 04 59 e5 cf 06 00 00  00 53 63 68 6f 6f 6c d3  |..Y......School.|
00000120  83 72 86 9e 97 33 b0 a7  10 f9 2b 55 6f 65 6d ec  |.r...3....+Uoem.|
00000130  b2 98 97 3a 5f 09 52 b0  15 42 72 6f ca 61 56 03  |...:_.R..Bro.aV.|
00000140  00 00 00 00 00 00 00 b5  ec 9f 5b 00 00 00 00 00  |..........[.....|
00000150  00 00 00 00 00 00 00 d3  83 72 86 9e 97 33 b0 a7  |.........r...3..|
00000160  10 f9 2b 55 6f 65 6d ec  b2 98 97 3a 5f 09 52 b0  |..+Uoem....:_.R.|
00000170  15 42 72 6f ca 61 56 02  00 00 00 6f 66 8f 9c 67  |.Bro.aV....of..g|
00000180  71 a1 2e 28 a6 73 a6 7c  93 c3 c3 fd 08 5a 51 b2  |q..(.s.|.....ZQ.|
00000190  3a 59 d7 fd 2d 9f 1a a2  c0 a9 ce da cc 04 00 00  |:Y..-...........|
000001a0  00 00 00 00 00 b5 ec 9f  5b 00 00 00 00 00 00 00  |........[.......|
000001b0  00 00 00 00 00 8f 9c 67  71 a1 2e 28 a6 73 a6 7c  |.......gq..(.s.||
000001c0  93 c3 c3 fd 08 5a 51 b2  3a 59 d7 fd 2d 9f 1a a2  |.....ZQ.:Y..-...|
000001d0  c0 a9 ce da cc 08 00 00  00 53 6f 66 74 77 61 72  |.........Softwar|
000001e0  65 7d cd 9c fd 6f a5 93  6b f1 dc 68 11 17 53 21  |e}...o..k..h..S!|
000001f0  dd 39 33 24 67 91 a6 89  0a 48 d1 76 ee af 41 20  |.93$g....H.v..A |
00000200  38 05 00 00 00 00 00 00  00 b5 ec 9f 5b 00 00 00  |8...........[...|
00000210  00 00 00 00 00 00 00 00  00 7d cd 9c fd 6f a5 93  |.........}...o..|
00000220  6b f1 dc 68 11 17 53 21  dd 39 33 24 67 91 a6 89  |k..h..S!.93$g...|
00000230  0a 48 d1 76 ee af 41 20  38 0b 00 00 00 45 6e 67  |.H.v..A 8....Eng|
00000240  69 6e 65 65 72 69 6e 67  41 d1 b1 78 69 0e 0b 03  |ineeringA..xi...|
00000250  2e b8 56 ed 58 dd 78 70  99 df bb cf 42 db 6a 4f  |..V.X.xp....B.jO|
00000260  13 cb 89 37 42 7e b7 d2                           |...7B~..|
00000268
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./blockchain_deserialize-test
Blockchain: {
    chain [6]: [
        Block: {
            info: { 0, 0, 1537578000, 0, 0000000000000000000000000000000000000000000000000000000000000000 },
            data: { "Holberton School", 16 },
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: { 1, 0, 1537207477, 0, c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 },
            data: { "Holberton", 9 },
            hash: 78d9897a5631e0841101a291e8040fd525cd72ddbf649733104bdd0a0459e5cf
        }
        Block: {
            info: { 2, 0, 1537207477, 0, 78d9897a5631e0841101a291e8040fd525cd72ddbf649733104bdd0a0459e5cf },
            data: { "School", 6 },
            hash: d38372869e9733b0a710f92b556f656decb298973a5f0952b01542726fca6156
        }
        Block: {
            info: { 3, 0, 1537207477, 0, d38372869e9733b0a710f92b556f656decb298973a5f0952b01542726fca6156 },
            data: { "of", 2 },
            hash: 8f9c6771a12e28a673a67c93c3c3fd085a51b23a59d7fd2d9f1aa2c0a9cedacc
        }
        Block: {
            info: { 4, 0, 1537207477, 0, 8f9c6771a12e28a673a67c93c3c3fd085a51b23a59d7fd2d9f1aa2c0a9cedacc },
            data: { "Software", 8 },
            hash: 7dcd9cfd6fa5936bf1dc6811175321dd3933246791a6890a48d176eeaf412038
        }
        Block: {
            info: { 5, 0, 1537207477, 0, 7dcd9cfd6fa5936bf1dc6811175321dd3933246791a6890a48d176eeaf412038 },
            data: { "Engineering", 11 },
            hash: 41d1b178690e0b032eb856ed58dd787099dfbbcf42db6a4f13cb8937427eb7d2
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```

### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/blockchain_deserialize.c`

---

### [7. Block validity]()

Write a function that verifies that a Block is valid

* Prototype: `int block_is_valid(block_t const *block, block_t const *prev_block)`;, where:
    * `block` points to the Block to check
    * `prev_block` points to the previous Block in the Blockchain, or is NULL if block is the first `Block` of the chain
* The following requirements must be fulfilled for a Block to be valid:
    * `block` should not be `NULL`
    * `prev_block` should be `NULL` ONLY if `block`‘s index is 0
    * If the Block’s index is 0, the Block should match the Genesis Block described in the first task
    * The Block’s index must be the previous Block’s index, plus 1
    * The computed hash of the previous block must match the one it stores
    * The computed hash of the previous block must match the reference one stored in block
    * The computed hash of the Block must match the one it stores
    * You don’t have to worry about the **timestamp** and the **difficulty** for this iteration of the Blockchain.
    * The Block `data` length must not exceed **BLOCKCHAIN_DATA_MAX**

```
alex@~/holbertonschool-blockchain/blockchain/v0.1$ cat test/block_is_valid-main.c
#include <stdlib.h>
#include <stdio.h>

#include "blockchain.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *first, *block;

    blockchain = blockchain_create();
    first = llist_get_head(blockchain->chain);

    block = block_create(first, (int8_t *)"Holberton", 9);
    block_hash(block, block->hash);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

    if (block_is_valid(first, NULL) != 0 ||
    block_is_valid(block, first) != 0)
    {
        fprintf(stderr, "Block invalid\n");
        return (EXIT_FAILURE);
    }
    printf("Block is valid\n");

    blockchain_destroy(blockchain);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.1$ gcc -Wall -Wextra -Werror -pedantic -I. -o block_is_valid-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c test/block_is_valid-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.1$ ./block_is_valid-test
Block is valid
alex@~/holbertonschool-blockchain/blockchain/v0.1$
```
### Repo:

* GitHub repository:  `holbertonschool-blockchain`
* File: `blockchain/v0.1/block_is_valid.c`
