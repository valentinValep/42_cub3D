#include "cub3d.h"

int	pass_spaces(char *line);

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
