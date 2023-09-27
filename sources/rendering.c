#include <math.h>
#include "cub3d.h"

#include <stdio.h>
// rename ? nearest_wall
t_nearest_wall	cast_ray(t_context *context, t_ray ray)
{
	t_vec2			side_dist;
	int				*wall;
	t_nearest_wall	res;
	const float		delta[2] = {
		fabs(1 / ray.dir.x),
		fabs(1 / ray.dir.y)
	};

	wall = (int [2]){ray.pos.x, ray.pos.y};
	side_dist.x = sqrtf(powf((ray.pos.x - wall[0]) / ray.dir.x * ray.dir.y, 2) + powf((ray.pos.x - wall[0]), 2));
	side_dist.y = sqrtf(powf((ray.pos.y - wall[1]) / ray.dir.y * ray.dir.x, 2) + powf((ray.pos.y - wall[1]), 2));
	//if (ray.dir.x < 0)
	//	side_dist.x = (ray.pos.x - wall[0]) * delta[0];
	//else
	//	side_dist.x = (wall[0] + 1.0 - ray.pos.x) * delta[0];
	//if (ray.dir.y < 0)
	//	side_dist.y = (ray.pos.y - wall[1]) * delta[1];
	//else
	//	side_dist.y = (wall[1] + 1.0 - ray.pos.y) * delta[1];
	res.perceived_distance = 0;
	while (get_map_char(&context->map, wall[0], wall[1]) != '1')
	{
		if (side_dist.x < side_dist.y)
		{
			wall[0] += 1 * ((ray.dir.x > 0) * 2 - 1);
			res.perceived_distance = side_dist.x;
			res.side = WEST * (ray.dir.x < 0) + EAST * !(ray.dir.x < 0);
			side_dist.x += delta[0];
		}
		else
		{
			wall[1] += 1 * ((ray.dir.y > 0) * 2 - 1);
			res.perceived_distance = side_dist.y;
			res.side = SOUTH * (ray.dir.y < 0) + NORTH * !(ray.dir.y < 0);
			side_dist.y += delta[1];
		}
	}
	return (res);
}

int	get_wall_pixel_color(t_context *context, t_nearest_wall wall, t_ray ray, int wall_height, int wall_start)
{
	//
}

void	draw_col(t_context *context, t_nearest_wall wall, t_ray ray, int col)
{
	int			i;
	int			wall_height;
	int			wall_start;

	if (col == 0 || col == 400)
		printf("col %d: %f\n", col, wall.perceived_distance);
	(void) context;
	if (!wall.perceived_distance)
		wall_height = WIN_HEIGHT;
	else
		wall_height = 1 / wall.perceived_distance * 50;
	i = 0;
	wall_start = (WIN_HEIGHT - wall_height) / 2;
	while (i < wall_start)
	{
		set_img_pixel(&context->img, col, i, context->map.ceil_color);
		i++;
	}
	while (i < wall_height + wall_start)
	{
		set_img_pixel(&context->img, col, i, get_wall_pixel_color(context, wall, ray, wall_height, wall_start));
		i++;
	}
	while (i < WIN_HEIGHT)
	{
		set_img_pixel(&context->img, col, i, context->map.ground_color);
		i++;
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
