#include "cub3d.h"
#include "libft.h"
#include <stdlib.h>

char	get_map_char(t_map *map, int x, int y)
{
	if (x < 0 || x >= map->width || y < 0 || y >= map->height)
		return (' ');
	return ((map->grid[y])[x]);
}

int	is_wall_map(t_map *map, int x, int y)
{
	return (get_map_char(map, x, y) == '1');
}

void	set_map_char(t_map *map, int x, int y, char c)
{
	(map->grid[y])[x] = c;
}

void	destroy_map(t_map *map, void *mlx)
{
	int	i;

	i = 0;
	while (i < map->height)
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	i = 0;
	while (i < 4)
	{
		if (map->textures[i].addr)
			mlx_destroy_image(mlx, map->textures[i].addr);
		i++;
	}
}
