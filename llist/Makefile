CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3

NAME = libllist.a
SRC = llist.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c llist.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(NAME)
