# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/11 15:51:22 by fguarrac          #+#    #+#              #
#    Updated: 2023/10/11 15:51:23 by fguarrac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := cub3D

CC := cc

# @TODO rm -g3
CFLAGS := -Wall -Werror -Wextra -MMD -g3
FSANITIZE := -fsanitize=leak,address,undefined

SOURCES_DIR := sources/
BINARIES_DIR := build/
INCLUDES_DIR := includes/
LIBRARIES_DIR := lib/

INCLUDES := -I$(INCLUDES_DIR) -I$(LIBRARIES_DIR)minilibx -I$(LIBRARIES_DIR)libft

LIBRARIES := -L$(LIBRARIES_DIR)minilibx -lmlx -L/usr/lib -lXext -lX11 -lm -L$(LIBRARIES_DIR)libft -lft

OBJ := context.o \
	context_utils.o \
	errors.o \
	images.o \
	inputs_computing.o \
	inputs.o \
	main.o \
	map_init.o \
	map_init_raw_grid.o \
	map_init_utils.o \
	map_parsing_color.o \
	map_parsing_init.o \
	map_parsing_texture.o \
	map_parsing_utils.o \
	map.o \
	player.o \
	rendering_draw.o \
	rendering_minimap.o \
	rendering.o \
	ft_draw_line_to_img.o \
	ft_draw_line_to_img_2.o

OBJ := $(addprefix $(BINARIES_DIR),$(OBJ))

DEPS := ${OBJ:.o=.d}

RM := rm -f

$(NAME): $(LIBRARIES_DIR)libft/libft.a $(LIBRARIES_DIR)minilibx/libmlx.a $(OBJ)
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
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) $(DEPS)
	$(RM) -r $(BINARIES_DIR)
	make -C $(LIBRARIES_DIR)libft clean
	make -C $(LIBRARIES_DIR)minilibx clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBRARIES_DIR)libft fclean

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re

-include $(DEPS)
