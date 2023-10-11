#include "cub3d.h"

void	destroy_textures(t_context *context)
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

void	replace_nl_by_zero(t_vector *raw_grid)
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

int	compute_map_width(t_vector raw_grid)
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
