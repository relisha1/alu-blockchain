CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g3
CFLAGS += -Wno-deprecated-declarations
CFLAGS += -I. -Itransaction/ -Iprovided/ -I../../crypto

NAME = libhblk_blockchain.a

SRC = blockchain_create.c block_create.c block_destroy.c \
      blockchain_destroy.c block_hash.c blockchain_serialize.c \
      blockchain_deserialize.c block_is_valid.c hash_matches_difficulty.c \
      blockchain_difficulty.c block_mine.c \
      transaction/tx_out_create.c transaction/unspent_tx_out_create.c \
      transaction/tx_in_create.c transaction/transaction_hash.c \
      transaction/tx_in_sign.c transaction/transaction_create.c \
      transaction/transaction_is_valid.c transaction/coinbase_create.c \
      transaction/coinbase_is_valid.c transaction/transaction_destroy.c \
      transaction/update_unspent.c
OBJ = $(SRC:.c=.o)

HEADERS = blockchain.h transaction/transaction.h provided/endianness.h

.PHONY: all clean

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(NAME)
