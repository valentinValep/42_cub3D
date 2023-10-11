/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:53:21 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:53:22 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "cub3d.h"

char	get_first_char(char *line);
int		pass_spaces(char *line);
int		parse_color(char *path, int *color);
int		init_texture(t_context *context, int face, char *path);

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

static int	parse_one_texture_init(
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
				print_error(
					"Multiple textures for the same face\n"), EXIT_FAILURE);
		context->map.texture_initialisation_steps[face] = 1;
		if (init_texture_and_color(
				context, face, (*line) + 2 + pass_spaces(*line)))
			return (free(*line),
				print_error("Failed to init texture\n"), EXIT_FAILURE);
		(*step)++;
		if (*step == 6)
			return (-1);
	}
	free(*line);
	*line = get_next_line(fd);
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
		ret = parse_one_texture_init(context, fd, &line, &step);
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
