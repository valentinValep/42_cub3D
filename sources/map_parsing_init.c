#include <stdlib.h>
#include "cub3d.h"

char	get_first_char(char *line);

static int	get_face(char *line)
{
	int			i;
	const int	face_chars[6] = {'N', 'S', 'E', 'W', 'C', 'F'};
	const int	faces[6] = {NORTH, SOUTH, EAST, WEST, CEIL, GROUND};
	int			face;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	if (!line[i] || !line[i + 1] || !line[i + 2]
		|| ((line[i] != 'N' || line[i + 1] != 'O'
				|| !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'S' || line[i + 1] != 'O'
				|| !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'E' || line[i + 1] != 'A'
				|| !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'W' || line[i + 1] != 'E'
				|| !is_whitespace_no_newline(line[i + 2]))
			&& (line[i] != 'C' || !is_whitespace_no_newline(line[i + 1]))
			&& (line[i] != 'F' || !is_whitespace_no_newline(line[i + 1]))))
		return (-2 + (!line[i] || line[i] == '\n'));
	face = 0;
	while (face < 6 && line[i] != face_chars[face])
		face++;
	return (faces[face]);
}

static int	init_texture(t_context *context, int face, char *path)
{
	int	i;

	i = 0;
	while (path[i] && !is_whitespace(path[i]))
		i++;
	if (is_whitespace(path[i]))
	{
		path[i++] = 0;
		while (path[i] && is_whitespace_no_newline(path[i]))
			i++;
		if (path[i] && path[i] != '\n')
			return (print_error(
					"Invalid character after texture path\n"), EXIT_FAILURE);
	}
	context->map.textures[face].addr = mlx_xpm_file_to_image(
			context->mlx, path, &context->map.textures[face].width,
			&context->map.textures[face].height);
	if (!context->map.textures[face].addr)
		return (print_error("Failed to load texture\n"), EXIT_FAILURE);
	context->map.textures[face].pixels = mlx_get_data_addr(
			context->map.textures[face].addr,
			&context->map.textures[face].bpp,
			&context->map.textures[face].line_len,
			&context->map.textures[face].endian);
	return (0);
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

static int	parse_color(char *path, int *color)
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

static int	init_texture_and_color(t_context *context, int face, char *path)
{
	while (*path && is_whitespace_no_newline(*path))
		path++;
	if (!*path || *path == '\n')
		return (print_error("Missing texture path\n"), EXIT_FAILURE);
	if (face < 4)
		return (init_texture(context, face, path));
	if (parse_color(path,
			(int *[2]){&context->map.ceil_color,
			&context->map.ground_color}[face == GROUND]))
		return (print_error("Failed to parse color\n"), EXIT_FAILURE);
	return (0);
}

static int	init_parse_textures_initialisation(
	t_context *context, int fd, char **line, char *step)
{
	int	i;

	*step = 0;
	*line = get_next_line(fd);
	i = 0;
	while (i < 4)
	{
		context->map.textures[i].addr = NULL;
		i++;
	}
	if (!*line)
		return (print_error("Empty file\n"), EXIT_FAILURE);
	return (0);
}

static int	parse_texture_init(
	t_context *context, int fd, char **line, char *step)
{
	int			face;

	face = get_face(*line);
	if (face == -2)
		return (free(*line), print_error("Invalid texture\n"), EXIT_FAILURE);
	if (face >= 0)
	{
		if (context->map.texture_initialisation_steps[face])
			return (free(*line),
				print_error("Multiple textures for the same face\n"), EXIT_FAILURE);
		context->map.texture_initialisation_steps[face] = 1;
		if (init_texture_and_color(
				context, face, (*line) + 2 + pass_spaces(*line)))
			return (free(*line), print_error("Failed to init texture\n"), EXIT_FAILURE);
		(*step)++;
		if (*step == 6)
			return (-1);
	}
	free(*line);
	*line = get_next_line(fd);
	return (0);
}

int	parse_textures_init(t_context *context, int fd)
{
	char		*line;
	char		step;
	int			ret;

	context->map.texture_initialisation_steps = (char [6]){0};
	if (init_parse_textures_initialisation(context, fd, &line, &step))
		return (1);
	while (line && get_first_char(line) != '1' && get_first_char(line) != '0')
	{
		ret = parse_texture_init(context, fd, &line, &step);
		if (ret == 1)
			return (1);
		else if (ret == -1)
			break ;
	}
	if (line)
		free(line);
	if (step < 6)
		return (print_error("Missing textures\n"), EXIT_FAILURE);
	return (0);
}
