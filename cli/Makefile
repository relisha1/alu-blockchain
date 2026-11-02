CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CFLAGS += -Wno-deprecated-declarations

BC = ../blockchain/v0.3
CPPFLAGS = -I. -I$(BC) -I$(BC)/transaction -I$(BC)/provided \
	   -I../crypto -I../llist
LDLIBS = -L$(BC) -lhblk_blockchain -L../crypto -lhblk_crypto \
	 -L../llist -lllist -lssl -lcrypto -pthread

NAME = cli
SRC = cli.c cli_exit.c cli_help.c cli_info.c cli_load.c cli_mine.c \
      cli_save.c cli_send.c cli_wallet_load.c cli_wallet_save.c \
      find_command.c get_commands.c state_clear.c state_init.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean deps

all: deps $(NAME)

# Build the libraries the CLI links against
deps:
	$(MAKE) -C ../llist
	$(MAKE) -C ../crypto
	$(MAKE) -C $(BC)

$(NAME): $(OBJ) $(BC)/libhblk_blockchain.a
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c cli.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(NAME)
