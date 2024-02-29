CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -lssl -lcrypto

SRCS := sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJS := $(SRCS:.c=.o)
LIB := libhblk_crypto.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(LIB)

aclean: fclean all

.PHONY: all clean fclean aclean