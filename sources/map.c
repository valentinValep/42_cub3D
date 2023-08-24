#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "cub3D.h"
#include "libft.h"

int	verif_and_init_line(t_map *map, char *line)
{
	int	i;
	int	width;

	i = 0;
	width = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ')
		{
			width++;
			if (line[i] == '1' || line[i] == '0')
				;
			else if (line[i] == 'N' || line[i] == 'S'
				|| line[i] == 'E' || line[i] == 'W')
			{
				if (map->has_player)
					return (basic_error("Multiple player\n", 1));
				map->has_player = 1;
			}
			else
				return (basic_error("Invalid character in map\n", 1));
		}
		i++;
	}
	if (map->width == -1)
		map->width = width;
	else if (map->width != width)
		return (basic_error("Invalid map width\n", 1));
	map->height++;
	return (0);
}

int	insert_line_to_grid(t_vector *grid, char *line)
{
	int			i;
	t_vector	line_vec;

	i = 0;
	init_vec(&line_vec, sizeof(char));
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ')
		{
			if (push_vec(&line_vec, &line[i]))
				return (basic_error("Failed to pushback to line_vec\n", 1));
		}
		i++;
	}
	if (push_vec(grid, &line_vec))
		return (basic_error("Failed to pushback to grid\n", 1));
	return (0);
}

int	parse_line(t_map *map, char *line)
{
	if (verif_and_init_line(map, line))
		return (1);
	if (insert_line_to_grid(&map->grid, line))
		return (1);
	return (0);
}

int	init_map(t_map *map, char *path)
{
	int		fd;
	char	*line;

	map->width = -1;
	map->height = 0;
	map->has_player = 0;
	init_vec(&map->grid, sizeof(t_vector));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (basic_error("Failed to open file\n", 1));
	while ((line = get_next_line(fd)))
	{
		if (parse_line(map, line))
			return (1);
	}
	if (map->width == -1)
		return (basic_error("Empty map\n", 1));
	if (!map->has_player)
		return (basic_error("No player\n", 1));
	printf("width: %d\n", map->width);
	printf("height: %d\n", map->height);
	return (0);
}
