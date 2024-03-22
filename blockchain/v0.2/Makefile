# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -pedantic -Wno-deprecated-declarations -ggdb3 -std=gnu90

# Source files
LIBS = -I. -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Object files (generated from source files)
FILES_PROVIDED = provided/_blockchain_print.c provided/_blockchain_destroy.c provided/_endianness.c hash_matches_difficulty.c

# Default Target: Build the executable
TASK0 = blockchain_create
TASK1 = block_create
TASK2 = block_destroy
TASK3 = blockchain_destroy
TASK4 = block_hash
TASK5 = blockchain_serialize
TASK6 = blockchain_deserialize
TASK7 = block_is_valid
TASK00 = hash_matches_difficulty
TASK01 = block_mine
TASK02 = blockchain_difficulty


# Rule to build the executable
blockchain_create-test: $(TASK0).c test/blockchain_create-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_create-test: $(TASK1).c $(TASK0).c test/block_create-main.c $(PROFILES_PROVIDEDVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_destroy-test: $(TASK2).c $(TASK1).c $(TASK0).c test/block_destroy-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

blockchain_destroy-test: $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/blockchain_destroy-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_hash-test: $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/block_hash-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

blockchain_serialize-test: $(TASK5).c $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/blockchain_serialize-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

blockchain_deserialize-test: $(TASK6).c $(TASK5).c $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/blockchain_deserialize-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_is_valid-test: $(TASK7).c $(TASK6).c $(TASK5).c $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/block_is_valid-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

hash_matches_difficulty-test: $(TASK00).c test/hash_matches_difficulty-main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

block_mine-test: $(TASK01).c $(TASK7).c $(TASK6).c $(TASK5).c $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/block_mine-main.c $(FILES_PROVIDED)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

blockchain_difficulty-test: $(TASK02).c $(TASK7).c $(TASK6).c $(TASK5).c $(TASK4).c $(TASK3).c $(TASK2).c $(TASK1).c $(TASK0).c test/blockchain_difficulty-main.c $(FILES_PROVIDED)

# Clean up object files and the executable
clean:
	rm $(OBJ) \
	blockchain_create-test \
	block_create-test \
	block_destroy-test \
	blockchain_destroy-test \
	block_hash-test \
	blockchain_serialize-test \
	blockchain_deserialize-test \
	block_is_valid-test \
	hash_matches_difficulty-test \
	block_mine-test \
	blockchain_difficulty-test

# Check for betty style
check:
	betty $(TASK0).c $(TASK1).c $(TASK2).c $(TASK3).c $(TASK4).c $(TASK5).c $(TASK6).c $(TASK7).c $(TASK00).c $(TASK01).c $(TASK02).c

# Declaration of phony targets
.PHONY: clean, check
