CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3

NAME = libllist.a
SRC = llist.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean install

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c llist.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(NAME)

# Install system-wide so `#include <llist.h>` and `-lllist` work like on the
# ALX checker:  sudo make install
PREFIX ?= /usr/local
install: $(NAME)
	install -d $(PREFIX)/include $(PREFIX)/lib
	install -m 644 llist.h $(PREFIX)/include/llist.h
	install -m 644 $(NAME) $(PREFIX)/lib/$(NAME)
