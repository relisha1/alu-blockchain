CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CFLAGS += -Wno-deprecated-declarations
CPPFLAGS = -I. -Itransaction -Iprovided -I../../crypto -I../../llist
LDLIBS = -L. -lhblk_blockchain -L../../crypto -lhblk_crypto \
	 -L../../llist -lllist -lssl -lcrypto -pthread

NAME = libhblk_blockchain.a
SRC = blockchain_create.c blockchain_deserialize.c blockchain_destroy.c \
      blockchain_difficulty.c blockchain_serialize.c block_create.c \
      block_destroy.c block_hash.c block_is_valid.c block_mine.c \
      hash_matches_difficulty.c \
      transaction/coinbase_create.c transaction/coinbase_is_valid.c \
      transaction/transaction_create.c transaction/transaction_destroy.c \
      transaction/transaction_hash.c transaction/transaction_is_valid.c \
      transaction/tx_in_create.c transaction/tx_in_sign.c \
      transaction/tx_out_create.c transaction/unspent_tx_out_create.c \
      transaction/update_unspent.c \
      provided/endianness.c provided/_print_hex_buffer.c
OBJ = $(SRC:.c=.o)
HDR = blockchain.h transaction/transaction.h provided/provided.h \
      provided/endianness.h

.PHONY: all clean deps test

all: $(NAME)

deps:
	$(MAKE) -C ../../llist
	$(MAKE) -C ../../crypto

$(NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Build and link a test program: make test MAIN=path/to/some-main.c
test: deps $(NAME)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(MAIN) -o $(basename $(notdir $(MAIN))) $(LDLIBS)

clean:
	$(RM) $(OBJ) $(NAME)
