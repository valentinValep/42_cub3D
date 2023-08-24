NAME := cub3D

CC := cc

# @TODO rm -g3
FLAGS := -Wall -Werror -Wextra -MMD -g3

SOURCES_DIR := sources/
BINARIES_DIR := build/
INCLUDES_DIR := includes/
LIBRARIES_DIR := lib/

INCLUDES := -I$(INCLUDES_DIR) -I$(LIBRARIES_DIR)minilibx -I$(LIBRARIES_DIR)libft

LIBRARIES := -L$(LIBRARIES_DIR)minilibx -lmlx -L/usr/lib -lXext -lX11 -lm -lz -L$(LIBRARIES_DIR)libft -lft

OBJ := main.o \
	error.o \
	map.o \

OBJ := $(addprefix $(BINARIES_DIR),$(OBJ))

DEPS := ${OBJ:.o=.d}

RM := rm -f

$(NAME) : $(OBJ)
	make -C $(LIBRARIES_DIR)libft
	make -C $(LIBRARIES_DIR)minilibx
	$(CC) $(OBJ) $(LIBRARIES) -o $(NAME)

$(BINARIES_DIR) :
	mkdir $(BINARIES_DIR)

$(BINARIES_DIR)%.o : $(SOURCES_DIR)%.c | $(BINARIES_DIR)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

all : $(NAME)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re

-include $(DEPS)
