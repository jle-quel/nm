NAME = ft_nm

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = inc

SRC_NAME =	error.c					\
			lib.c					\
			main.c					\
			nm.c					\
			object.c				\
			render.c				\
			tree.c					\
			utils.c					\
			symtab.c				\
			type.c					\
			macho64.c				\
			macho32.c				\
			stat64.c				\
			stat32.c				\
			fat.c					\
			archive.c				\

OBJ_NAME = $(SRC_NAME:.c=.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror 

RED=\033[1;31m
GREEN=\033[1;32m
NC=\033[0m

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c Makefile
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_PATH)
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"

clean:
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@echo "$(RED)[-]$(NC) Objects cleaned"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)[-]$(NC) Program clear"
	@rm -rf $(LINK_NAME)
	@echo "$(RED)[-]$(NC) Link clear"

re:fclean
	@make
