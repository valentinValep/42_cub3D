#include "cub3d.h"
#include <stdlib.h>

void	destroy_line_grid(void *line);
void	destroy_init_map_grid(char **grid, int i);
char	get_first_char(char *line);
void	replace_nl_by_zero(t_vector *raw_grid);
int		compute_map_width(t_vector raw_grid);

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

int	transform_raw_grid(t_vector raw_grid, t_map *map)
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
