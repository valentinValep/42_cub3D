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
	errors.o \
	map.o \
	rendering2.o \
	rendering_minimap.o \
	images.o \
	inputs.o \

OBJ := $(addprefix $(BINARIES_DIR),$(OBJ))

DEPS := ${OBJ:.o=.d}

RM := rm -f

$(NAME): $(OBJ) $(LIBRARIES_DIR)libft/libft.a $(LIBRARIES_DIR)minilibx/libmlx.a
	$(CC) $(OBJ) $(LIBRARIES) -o $(NAME)

bonus: $(NAME)

$(LIBRARIES_DIR)libft/libft.a:
	git submodule update --init --recursive
	make -C $(LIBRARIES_DIR)libft

$(LIBRARIES_DIR)minilibx/libmlx.a:
	git submodule update --init --recursive
	make -C $(LIBRARIES_DIR)minilibx

$(BINARIES_DIR):
	mkdir $(BINARIES_DIR)

$(BINARIES_DIR)%.o: $(SOURCES_DIR)%.c | $(BINARIES_DIR)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) $(DEPS)
	make -C $(LIBRARIES_DIR)libft clean
	make -C $(LIBRARIES_DIR)minilibx clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBRARIES_DIR)libft fclean

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re

-include $(DEPS)
