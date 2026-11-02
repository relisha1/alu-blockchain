CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CFLAGS += -Wno-deprecated-declarations
CPPFLAGS = -I. -Iprovided -I../../crypto -I../../llist
LDLIBS = -L. -lhblk_blockchain -L../../crypto -lhblk_crypto \
	 -L../../llist -lllist -lssl -lcrypto -pthread

NAME = libhblk_blockchain.a
SRC = blockchain_create.c block_create.c block_destroy.c \
      blockchain_destroy.c block_hash.c blockchain_serialize.c \
      blockchain_deserialize.c block_is_valid.c \
      provided/endianness.c provided/_print_hex_buffer.c \
      provided/_blockchain_print.c
OBJ = $(SRC:.c=.o)
HDR = blockchain.h provided/provided.h provided/endianness.h

.PHONY: all clean deps

all: $(NAME)

deps:
	$(MAKE) -C ../../llist
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
