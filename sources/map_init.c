#include "cub3d.h"
#include <stdlib.h>
#include <fcntl.h>

int		parse_textures_init(t_context *context, int fd);
char	get_first_char(char *line);

static void	destroy_textures(t_context *context)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (context->map.textures[i].addr)
			mlx_destroy_image(context->mlx, context->map.textures[i].addr);
		i++;
	}
}

static void	destroy_line_grid(void *line)
{
	free(*(char **)line);
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

static void	trim_raw_grid(t_vector *raw_grid)
{
	char	*line;

	while (raw_grid->len > 0)
	{
		line = ((char **)raw_grid->tab)[0];
		if (!get_first_char(line))
		{
			free(line);
			remove_vec(raw_grid, 0);
		}
		else
			break ;
	}
	while (raw_grid->len > 0)
	{
		line = ((char **)raw_grid->tab)[(raw_grid->len - 1)];
		if (!get_first_char(line))
		{
			free(line);
			remove_vec(raw_grid, (raw_grid->len - 1));
		}
		else
			break ;
	}
}

static void	replace_nl_by_zero(t_vector *raw_grid)
{
	char	*c;
	int		i;

	i = 0;
	while (i < raw_grid->len)
	{
		c = &((char **)raw_grid->tab)[i][
			ft_strlen(((char **)raw_grid->tab)[i]) - 1];
		if (*c == '\n')
			*c = 0;
		i++;
	}
}

static int	compute_map_width(t_vector raw_grid)
{
	int		i;
	size_t	max_width;

	i = 0;
	max_width = 0;
	while (i < raw_grid.len)
	{
		if (ft_strlen(((char **)raw_grid.tab)[i]) > max_width)
			max_width = ft_strlen(((char **)raw_grid.tab)[i]);
		i++;
	}
	return (max_width);
}

static void	destroy_init_map_grid(char **grid, int i)
{
	while (i >= 0)
	{
		free(grid[i]);
		i--;
	}
	free(grid);
}

static int	transform_raw_line(char *dest, char *src, t_map *map, int y)
{
	int	i;

	i = -1;
	while (++i < map->width)
	{
		dest[i] = ' ';
		if (src[i] == 'N' || src[i] == 'S' || src[i] == 'E' || src[i] == 'W')
		{
			if (map->has_player)
				return (print_error("Multiple player\n"), EXIT_FAILURE);
			init_player(map, src[i], (t_vec2){i, y});
			dest[i] = '0';
		}
		else if (src[i] == '1')
			dest[i] = '1';
		else if (!src[i])
			break ;
		else if (src[i] == '0')
			dest[i] = '0';
		else if (!is_whitespace_no_newline(src[i]))
			return (print_error("Invalid character in map\n"), EXIT_FAILURE);
	}
	while (i < map->width)
		dest[i++] = ' ';
	return (0);
}

static int	init_map_grid(t_vector raw_grid, t_map *map)
{
	int	i;

	map->grid = malloc(sizeof(char *) * map->height);
	if (!map->grid)
		return (print_error("Map memory allocation error\n"), EXIT_FAILURE);
	i = 0;
	while (i < map->height)
	{
		map->grid[i] = malloc(map->width * sizeof(char));
		if (!map->grid[i])
			return (destroy_init_map_grid(map->grid, i - 1),
				print_error("Map memory allocation error\n"), EXIT_FAILURE);
		if (transform_raw_line(map->grid[i],
				((char **)raw_grid.tab)[i], map, i))
			return (destroy_init_map_grid(map->grid, i), 1);
		i++;
	}
	return (0);
}

static int	transform_raw_grid(t_vector raw_grid, t_map *map)
{
	if (!raw_grid.tab)
		return (print_error("Map memory allocation error\n"), EXIT_FAILURE);
	replace_nl_by_zero(&raw_grid);
	trim_raw_grid(&raw_grid);
	map->height = raw_grid.len;
	if (map->height == 0)
		return (destroy_vec2(&raw_grid, destroy_line_grid),
			print_error("Empty map\n"), EXIT_FAILURE);
	map->width = compute_map_width(raw_grid);
	if (map->width == 0)
		return (destroy_vec2(&raw_grid, destroy_line_grid),
			print_error("Empty map\n"), EXIT_FAILURE);
	if (init_map_grid(raw_grid, map))
		return (destroy_vec2(&raw_grid, destroy_line_grid), 1);
	destroy_vec2(&raw_grid, destroy_line_grid);
	return (0);
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
