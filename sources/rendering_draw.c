#include "cub3d.h"

static float	get_wall_texture_x(t_nearest_wall wall, t_ray ray)
{
	float	wall_texture_x;

	if (wall.side == NORTH || wall.side == SOUTH)
	{
		wall_texture_x = ray.pos.x + wall.perceived_distance * ray.dir.x;
		wall_texture_x -= (int)wall_texture_x;
	}
	else
	{
		wall_texture_x = ray.pos.y + wall.perceived_distance * ray.dir.y;
		wall_texture_x -= (int)wall_texture_x;
	}
	if (wall.side == EAST || wall.side == NORTH)
		wall_texture_x = 1 - wall_texture_x;
	return (wall_texture_x);
}

static void	draw_black_col(t_context *context, int col)
{
	int	i;

	i = 0;
	while (i < context->win_height)
	{
		set_img_pixel(&context->img, col, i, 0);
		i++;
	}
}

void	draw_col(t_context *context, t_nearest_wall wall, t_ray ray, int col)
{
	int			row;
	const int	line_height = 1 / wall.perceived_distance
		* (WALL_HEIGHT * WIN_SIZE_PROPORTION);
	const int	line_start = (context->win_height - line_height) / 2;

	if (!wall.perceived_distance)
		return ((void) draw_black_col(context, col));
	row = 0;
	while (row < context->win_height)
	{
		if (row < line_start)
			set_img_pixel(&context->img, col, row, context->map.ceil_color);
		else if (row < line_height + line_start)
			set_img_pixel(&context->img, col, row,
				get_img_pixel(&context->map.textures[wall.side],
					get_wall_texture_x(wall, ray)
					* context->map.textures[wall.side].width,
					((row - line_start) / (float) line_height)
					* context->map.textures[wall.side].height));
		else if (row < context->win_height)
			set_img_pixel(&context->img, col, row, context->map.ground_color);
		row++;
	}
}
