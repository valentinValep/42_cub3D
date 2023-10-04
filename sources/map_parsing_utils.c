#include "cub3d.h"

char	get_first_char(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	return (line[i]);
}
