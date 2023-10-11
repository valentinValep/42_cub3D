/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:52:52 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:52:53 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdlib.h>
#include <fcntl.h>

int		parse_textures_init(t_context *context, int fd);
char	get_first_char(char *line);
int		transform_raw_grid(t_vector raw_grid, t_map *map);
void	destroy_textures(t_context *context);

void	destroy_line_grid(void *line)
{
	free(*(char **)line);
}

void	destroy_init_map_grid(char **grid, int i)
{
	while (i >= 0)
	{
		free(grid[i]);
		i--;
	}
	free(grid);
}

static t_vector	get_raw_grid(int fd)
{
	t_vector	res;
	char		*line;

	init_vec(&res, sizeof(char *));
	line = get_next_line(fd);
	if (!line)
		return (destroy_vec2(&res, &destroy_line_grid),
			print_error("Empty map\n"), res);
	while (line)
	{
		if (push_vec(&res, &line))
			return (gnl_close(fd), destroy_vec2(&res, &destroy_line_grid),
				print_error("Failed to pushback to raw_grid\n"), res);
		line = get_next_line(fd);
	}
	return (res);
}

static int	check_map_closure(t_map *map)
{
	int	i;
	int	j;

	j = 0;
	while (j < map->height)
	{
		i = 0;
		while (i < map->width)
		{
			if (get_map_char(map, i, j) == '0')
			{
				if (is_whitespace_no_newline(get_map_char(map, i + 1, j)))
					return (print_error("Map not closed\n"), EXIT_FAILURE);
				if (is_whitespace_no_newline(get_map_char(map, i - 1, j)))
					return (print_error("Map not closed\n"), EXIT_FAILURE);
				if (is_whitespace_no_newline(get_map_char(map, i, j + 1)))
					return (print_error("Map not closed\n"), EXIT_FAILURE);
				if (is_whitespace_no_newline(get_map_char(map, i, j - 1)))
					return (print_error("Map not closed\n"), EXIT_FAILURE);
			}
			i++;
		}
		j++;
	}
	return (0);
}

int	init_map(t_context *context, char *path)
{
	int	fd;

	context->map.has_player = 0;
	if (ft_strlen(path) < 4 || ft_strcmp(path + ft_strlen(path) - 4, ".cub"))
		return (print_error("Invalid map file\n"), EXIT_FAILURE);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (print_error("Failed to open file\n"), EXIT_FAILURE);
	if (parse_textures_init(context, fd))
		return (destroy_textures(context), gnl_close(fd),
			print_error("Failed to initialise textures and colors\n"),
			EXIT_FAILURE);
	if (transform_raw_grid(get_raw_grid(fd), &context->map))
		return (destroy_textures(context),
			print_error("Map initialisation Error\n"), EXIT_FAILURE);
	if (!context->map.has_player)
		return (destroy_textures(context),
			destroy_init_map_grid(context->map.grid, context->map.height - 1),
			print_error("No player\n"), EXIT_FAILURE);
	if (check_map_closure(&context->map))
		return (destroy_textures(context),
			destroy_init_map_grid(context->map.grid, context->map.height - 1),
			print_error("Error during map closure check\n"), EXIT_FAILURE);
	return (0);
}
