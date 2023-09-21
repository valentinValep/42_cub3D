#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"
#include "libft.h"

int	get_face(char *line)
{
	int			i;
	const int	face_chars[6] = {'N', 'S', 'E', 'W', 'C', 'F'};
	const int	faces[6] = {NORTH, SOUTH, EAST, WEST, CEIL, GROUND};
	int			face;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	if (!line[i] || !line[i + 1] || !line[i + 2]
		|| ((line[i] != 'N' || line[i + 1] != 'O' || !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'S' || line[i + 1] != 'O' || !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'E' || line[i + 1] != 'A' || !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'W' || line[i + 1] != 'E' || !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'C' || !is_whitespace_no_newline(line[i + 1]))
			&& (line[i] != 'F' || !is_whitespace_no_newline(line[i + 1]))))
		return (-1);
	face = 0;
	while (face < 6 && line[i] != face_chars[face])
		face++;
	return (faces[face]);
}

char	get_first_char(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	return (line[i]);
}

static int	pass_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	return (i);
}

static int	pass_atoi_number(char *line)
{
	int	i;

	i = 0;
	if (line[i] == '-' || line[i] == '+')
		i++;
	while (line[i] && line[i] >= '0' && line[i] <= '9')
		i++;
	return (i);
}

static int	parse_one_color(char **path, int *color, int shift)
{
	int	current_color;

	current_color = ft_atoi((*path));
	if (!(ft_isdigit(*(*path)) || (*(*path) == '+'
				&& ft_isdigit(*((*path) + 1))))
		|| current_color < 0
		|| current_color > 255)
		return (1);
	*color += current_color << shift;
	(*path) += pass_atoi_number((*path));
	(*path) += pass_spaces((*path));
	return (0);
}

int	parse_color(char *path, int *color)
{
	*color = 0;
	if (parse_one_color(&path, color, 16) || *path != ',')
		return (1);
	path++;
	path += pass_spaces(path);
	if (parse_one_color(&path, color, 8) || *path != ',')
		return (1);
	path++;
	path += pass_spaces(path);
	if (parse_one_color(&path, color, 0) || (*path && *path != '\n'))
		return (1);
	return (0);
}

int	init_texture_and_color(t_context *context, int face, char *path)
{
	int	i;

	i = 0;
	while (*path && is_whitespace_no_newline(*path))
		path++;
	if (!*path || *path == '\n')
		return (basic_error("Missing texture path\n", 1));
	if (face < 4)
	{
		while (path[i] && !is_whitespace(path[i]))
			i++;
		if (is_whitespace(path[i]))
		{
			path[i++] = 0;
			while (path[i] && is_whitespace_no_newline(path[i]))
				i++;
			if (path[i] && path[i] != '\n')
				return (basic_error("Invalid character after texture path\n", 1));
		}
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
				return (free(line), basic_error("Multiple textures for the same face\n", 1));
			steps[face] = 1;
			if (init_texture_and_color(context, face, line + 2 + pass_spaces(line)))
				return (free(line), basic_error("Failed to init texture\n", 1));
			step++;
			if (step == 6)
				break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	if (step < 6)
		return (basic_error("Missing textures\n", 1));
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

void	init_player(t_map *map, char c, t_vec2 pos)
{
	map->player.pos = (t_vec2){pos.x + 0.5, pos.y + 0.5};
	map->player.speed = (t_vec2){0., 0.};
	map->player.rotate = 0.;
	map->player.dir = get_dir_from_char(c);
	map->player.plane = (t_vec2){-map->player.dir.y, map->player.dir.x};
	map->has_player = 1;
}

static void destroy_raw_grid(t_vector *raw_grid)
{
	int	i;

	i = 0;
	while (i < raw_grid->len)
	{
		free(((char **)raw_grid->tab)[i]);
		i++;
	}
	raw_grid->tab = NULL;
	destroy_vec(raw_grid);
}

static t_vector	get_raw_grid(int fd)
{
	t_vector	res;
	char		*line;

	init_vec(&res, sizeof(char *));
	line = get_next_line(fd);
	if (!line)
		return (destroy_raw_grid(&res), basic_error("Empty map\n", 0), res);
	while (line)
	{
		if (push_vec(&res, &line))
			return (destroy_raw_grid(&res), basic_error("Failed to pushback to raw_grid\n", 0), res);
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

	i = 0;
	while (i < map->width)
	{
		if (is_whitespace_no_newline(src[i]) || src[i] == '0')
			dest[i] = '0';
		else if (src[i] == 'N' || src[i] == 'S'
			|| src[i] == 'E' || src[i] == 'W')
		{
			if (!map->has_player)
				init_player(map, src[i], (t_vec2){i, y});
			else
				return (basic_error("Multiple player\n", 1));
		}
		else if (src[i] == '1')
			dest[i] = 1;
		else
			return (basic_error("Invalid character in map\n", 1));
		dest[i] = src[i];
		i++;
	}
	return (0);
}

static int	init_map_grid(t_vector raw_grid, t_map *map)
{
	int	i;

	map->grid = malloc(sizeof(char *) * map->height);
	if (!map->grid)
		return (basic_error("Map memory allocation error\n", 1));
	i = 0;
	while (i < map->height)
	{
		map->grid[i] = malloc(sizeof(char) * map->width);
		if (!map->grid[i])
			return (destroy_init_map_grid(map->grid, i - 1),
				basic_error("Map memory allocation error\n", 1));
		if (transform_raw_line(map->grid[i],
				((char **)raw_grid.tab)[i], map, i))
			return (destroy_init_map_grid(map->grid, i - 1),
				basic_error("", 1));
		i++;
	}
	return (0);
}

static int transform_raw_grid(t_vector raw_grid, t_map *map)
{
	if (!raw_grid.tab)
		return (basic_error("Map memory allocation error\n", 1));
	replace_nl_by_zero(&raw_grid);
	trim_raw_grid(&raw_grid);
	map->height = raw_grid.len;
	if (map->height == 0)
		return (basic_error("Empty map\n", 1));
	map->width = compute_map_width(raw_grid);
	if (map->width == 0)
		return (basic_error("Empty map\n", 1));
	if (init_map_grid(raw_grid, map))
		return (basic_error("", 1));
	return (0);
}

int	init_map(t_context *context, char *path)
{
	int			fd;

	context->map.has_player = 0;
	if (ft_strlen(path) < 4 || ft_strcmp(path + ft_strlen(path) - 4, ".cub"))
		return (basic_error("Invalid map file\n", 1));
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (basic_error("Failed to open file\n", 1));
	if (parse_textures_init(context, fd))
		return (gnl_close(fd), basic_error("Failed to initialise textures and colors\n", 1));
	if (transform_raw_grid(get_raw_grid(fd), &context->map))
		return (basic_error("Map initialisation Error\n", 1));
	if (!context->map.has_player)
		return (basic_error("No player\n", 1));
	return (0);
}

char	get_map_char(t_map *map, int x, int y)
{
	if (x < 0 || x >= map->width || y < 0 || y >= map->height)
	{
		printf("Get map char error : x: %d, y: %d\n", x, y);
		return ('1');
	}
	return ((map->grid[y])[x]);
}

void	set_map_char(t_map *map, int x, int y, char c)
{
	(map->grid[y])[x] = c;
}
