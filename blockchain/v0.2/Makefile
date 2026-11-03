CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CFLAGS += -Wno-deprecated-declarations
CPPFLAGS = -I.
LDLIBS = -L. -lhblk_blockchain_v0.1 -L../../crypto -lhblk_crypto \
	 -lllist -lssl -lcrypto -pthread

NAME = libhblk_blockchain_v0.2.a
SRC = blockchain_create.c block_create.c block_destroy.c \
      blockchain_destroy.c block_hash.c blockchain_serialize.c \
      blockchain_deserialize.c block_is_valid.c hash_matches_difficulty.c \
      block_mine.c blockchain_difficulty.c \
      provided/_endianness.c provided/_blockchain_print.c \
      provided/_blockchain_destroy.c provided/_genesis.c
OBJ = $(SRC:.c=.o)
HDR = blockchain.h provided/endianness.h

.PHONY: all clean deps

all: $(NAME)

deps:
	$(MAKE) -C ../../crypto

$(NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Build and link a test program: make test MAIN=test/blockchain_create-main.c
test: deps $(NAME)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(MAIN) -o $(basename $(notdir $(MAIN))) $(LDLIBS)

clean:
	$(RM) $(OBJ) $(NAME)
