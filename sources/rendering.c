#include <math.h>
#include "cub3d.h"

#include <stdio.h>
// rename ? nearest_wall
t_nearest_wall	cast_ray(t_context *context, t_ray ray)
{
	t_vec2			side_dist;
	t_nearest_wall	res;
	int				*wall;
	const float		delta[2] = {
		fabs(1 / ray.dir.x),
		fabs(1 / ray.dir.y)
	};

	wall = (int [2]){ray.pos.x, ray.pos.y};
	side_dist.x = fabs((ray.dir.x > 0) - (ray.pos.x - wall[0])) * delta[0];
	side_dist.y = fabs((ray.dir.y > 0) - (ray.pos.y - wall[1])) * delta[1];
	res.perceived_distance = 0;
	res.side = 0;
	while (!is_wall_map(&context->map, wall[0], wall[1]))
	{
		if (side_dist.x < side_dist.y)
		{
			wall[0] += (ray.dir.x > 0) * 2 - 1;
			res.perceived_distance = side_dist.x;
			res.side = EAST * (ray.dir.x < 0) + WEST * !(ray.dir.x < 0);
			side_dist.x += delta[0];
		}
		else
		{
			wall[1] += (ray.dir.y > 0) * 2 - 1;
			res.perceived_distance = side_dist.y;
			res.side = SOUTH * (ray.dir.y < 0) + NORTH * !(ray.dir.y < 0);
			side_dist.y += delta[1];
		}
	}
	return (res);
}

float	get_wall_texture_x(t_nearest_wall wall, t_ray ray)
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

void	draw_black_col(t_context *context, int col)
{
	int	i;

	i = 0;
	while (i < WIN_HEIGHT)
	{
		set_img_pixel(&context->img, col, i, 0);
		i++;
	}
}

void	draw_col(t_context *context, t_nearest_wall wall, t_ray ray, int col)
{
	int			row;
	const int	line_height = 1 / wall.perceived_distance * WALL_HEIGHT;
	const int	line_start = (WIN_HEIGHT - line_height) / 2;

	if (!wall.perceived_distance)
		return ((void) draw_black_col(context, col));
	row = 0;
	while (row < WIN_HEIGHT)
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
		else if (row < WIN_HEIGHT)
			set_img_pixel(&context->img, col, row, context->map.ground_color);
		row++;
	}
}

void	raycaster(t_context *context, int col)
{
	t_ray			ray;
	t_nearest_wall	nearest_wall;

	ray.pos = context->map.player.pos;
	ray.dir = (t_vec2){context->map.player.dir.x
		+ ((float)col / WIN_WIDTH * context->map.player.plane.x)
		- context->map.player.plane.x / 2,
		context->map.player.dir.y
		+ ((float)col / WIN_WIDTH * context->map.player.plane.y)
		- context->map.player.plane.y / 2
	};
	nearest_wall = cast_ray(context, ray);
	draw_col(context, nearest_wall, ray, col);
}

void	render_main_scene(t_context *context)
{
	int	col;

	col = 0;
	while (col < WIN_WIDTH)
	{
		raycaster(context, col);
		col++;
	}
}
