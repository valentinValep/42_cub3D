#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "cub3d.h"
#include "libft.h"

int	get_face(char *line)
{
	int			i;
	const int	face_chars[6] = {'N', 'S', 'E', 'W', 'C', 'F'};
	const int	faces[6] = {NORTH, SOUTH, EAST, WEST, CEIL, GROUND};
	int			face;

	i = 0;
	while (line[i] && line[i] == ' ')
		i++;
	if (!line[i] || !line[i + 1] || !line[i + 2]
		|| ((line[i] != 'N' || line[i + 1] != 'O' || line[i + 2] != ' ')
			&& (line[i] != 'S' || line[i + 1] != 'O' || line[i + 2] != ' ')
			&& (line[i] != 'E' || line[i + 1] != 'A' || line[i + 2] != ' ')
			&& (line[i] != 'W' || line[i + 1] != 'E' || line[i + 2] != ' ')
			&& (line[i] != 'C' || line[i + 1] != ' ')
			&& (line[i] != 'F' || line[i + 1] != ' ')))
		return (-1);
	face = 0;
	while (face < 6 && line[i] != face_chars[face])
		face++;
	return (faces[face]);
}

int	get_first_char(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] == ' ')
		i++;
	return (line[i]);
}

int	parse_color(char *path, int *color)
{
	int	current_color;

	current_color = ft_atoi(path);
	if (!(*path >= '0' && *path <= '9') || current_color < 0 || current_color > 255)
		return (1);
	*color = current_color << 16;
	path = ft_strchr(path, ',');
	if (!path || !(*(path + 1) >= '0' && *(path + 1) <= '9'))
		return (1);
	current_color = ft_atoi(path + 1);
	if (current_color < 0 || current_color > 255)
		return (1);
	*color += current_color << 8;
	path = ft_strchr(path + 1, ',');
	if (!path || !(*(path + 1) >= '0' && *(path + 1) <= '9'))
		return (1);
	current_color = ft_atoi(path + 1);
	if (current_color < 0 || current_color > 255)
		return (1);
	*color += current_color;
	return (0);
}

int	init_texture_and_color(t_context *context, int face, char *path)
{
	int	i;

	i = 0;
	while (*path && *path == ' ')
		path++;
	if (!*path || *path == '\n')
		return (basic_error("Missing texture path\n", 1));
	while (path[i] && path[i] != ' ' && path[i] != '\n')
		i++;
	if (path[i] == ' ' || path[i] == '\n')
	{
		path[i++] = 0;
		while (path[i] && path[i] == ' ')
			i++;
		if (path[i])
			return (basic_error("Invalid character after texture path\n", 1));
	}
	if (face < 4)
	{
		context->map.textures[face].addr = mlx_xpm_file_to_image(
			context->mlx, path,
			&context->map.textures[face].width, &context->map.textures[face].height);
		if (!context->map.textures[face].addr)
			return (basic_error("Failed to load texture\n", 1));
		context->map.textures[face].pixels = mlx_get_data_addr(
			context->map.textures[face].addr,
			&context->map.textures[face].bpp,
			&context->map.textures[face].line_len,
			&context->map.textures[face].endian);
		return (0);
	}
	if (parse_color(path,
			(int *[2]){&context->map.ceil_color, &context->map.ground_color}[face == GROUND]))
		return (basic_error("Failed to parse color\n", 1));
	return (0);
}

int	parse_textures_init(t_context *context, int fd)
{
	char	*const steps = (char [6]){0, 0, 0, 0, 0, 0};
	char	*line;
	char	step;
	int		face;

	step = 0;
	line = get_next_line(fd);
	if (!line)
		return (basic_error("Empty file\n", 1));
	while (line && get_first_char(line) != '1' && get_first_char(line) != '0')
	{
		face = get_face(line);
		if (face >= 0)
		{
			if (steps[face])
				return (basic_error("Multiple textures for the same face\n", 1));
			steps[face] = 1;
			if (init_texture_and_color(context, face, line + 2))
				return (basic_error("Failed to init texture\n", 1));
			step++;
			if (step == 6)
				break ;
		}
		line = get_next_line(fd);
	}
	if (step < 6)
		return (basic_error("Missing textures\n", 1));
	return (0);
}

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

t_vec2	get_dir_from_char(char c)
{
	if (c == 'N')
		return ((t_vec2){0, -1});
	else if (c == 'S')
		return ((t_vec2){0, 1});
	else if (c == 'E')
		return ((t_vec2){1, 0});
	return ((t_vec2){-1, 0});
}

void	init_player(t_map *map)
{
	int		x;
	int		y;
	char	c;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			c = get_map_char(map, x, y);
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				map->player.pos = (t_vec2){x + 0.5, y + 0.5};
				map->player.speed = (t_vec2){0., 0.};
				map->player.rotate = 0.;
				map->player.dir = get_dir_from_char(c);
				map->player.plane = (t_vec2){-map->player.dir.y, map->player.dir.x};
				set_map_char(map, x, y, '0');
				return ;
			}
			x++;
		}
		y++;
	}
}

int	init_map(t_context *context, char *path)
{
	int		fd;
	char	*line;

	context->map.width = -1;
	context->map.height = 0;
	context->map.has_player = 0;
	init_vec(&context->map.grid, sizeof(t_vector));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (basic_error("Failed to open file\n", 1));
	if (parse_textures_init(context, fd))
		return (basic_error("Failed to initialise textures and colors\n", 1));
	while ((line = get_next_line(fd)))
	{
		if (parse_line(&context->map, line))
			return (1);
	}
	if (context->map.width == -1)
		return (basic_error("Empty map\n", 1));
	if (!context->map.has_player)
		return (basic_error("No player\n", 1));
	init_player(&context->map);
	return (0);
}

char	get_map_char(t_map *map, int x, int y)
{
	if (x < 0 || x >= map->width || y < 0 || y >= map->height)
	{
		printf("Get map char error : x: %d, y: %d\n", x, y);
		return ('1');
	}
	return (((char *)((t_vector *)map->grid.tab)[y].tab)[x]);
}

void	set_map_char(t_map *map, int x, int y, char c)
{
	((char *)((t_vector *)map->grid.tab)[y].tab)[x] = c;
}
