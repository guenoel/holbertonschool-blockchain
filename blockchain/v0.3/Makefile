# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -pedantic -g3

# Linker flags
LDFLAGS = -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Include flags
INCLUDES = -I. -Itransaction/ -Iprovided/ -I../../crypto

# Source files
FILES_PROVIDED = transaction/tx_out_create.c provided/_print_hex_buffer.c transaction/test/tx_out_create-main.c

# Object files
OBJECTS = $(FILES_PROVIDED:.c=.o)

# Target executable
TASK0 = transaction/tx_out_create-test

all: $(TASK0)

# Compile source files and link to create the executable
$(TASK0): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TASK0) $(OBJECTS)

check:
	betty transaction/tx_out_create.c

.PHONY: all clean check
