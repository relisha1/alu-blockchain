CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
# The EC_KEY API is deprecated (not removed) in OpenSSL 3; the project
# requires it, so silence the deprecation warnings instead of failing.
CFLAGS += -Wno-deprecated-declarations
OBJDIR = targets/

SRC0 = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c \
       ec_sign.c ec_verify.c
OBJ0 = $(addprefix $(OBJDIR), $(SRC0:%.c=%.o))
NAME0 = libhblk_crypto.a

.PHONY: all clean

all: $(NAME0)

$(NAME0): hblk_crypto.h $(OBJ0)
	ar rcs $(NAME0) $(OBJ0)

$(OBJDIR)%.o: %.c hblk_crypto.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR) $(NAME0)
