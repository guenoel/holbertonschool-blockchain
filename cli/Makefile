CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3 -Wno-deprecated-declarations -c -I. -I../blockchain/v0.3/ -I../blockchain/v0.3/transaction/ -I../blockchain/v0.3/provided/ -I../crypto
LIBS = -L../blockchain/v0.3 -lhblk_blockchain -L../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

SRC0 = $(wildcard *.c)
OBJ0 = $(SRC0:.c=.o)
NAME0 = cli

.PHONY: clean oclean flcean cli

clean:
	$(RM) *~ $(NAME0)

oclean:
	$(RM) $(OBJ0)

fclean: clean oclean

cli: cli.h $(OBJ0)
	$(CC) $(OBJ0) -o $(NAME0) $(LIBS)
