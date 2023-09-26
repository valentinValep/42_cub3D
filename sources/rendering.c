#include <math.h>
#include "cub3d.h"

#include <stdio.h>
// rename ? nearest_wall
t_nearest_wall	cast_ray(t_context *context, t_ray ray, int col)
{
	t_vec2			side_dist;
	int				*wall;
	t_nearest_wall	res;


	wall = (int [2]){ray.pos.x, ray.pos.y};
	side_dist.x = sqrtf(powf((ray.pos.x - wall[0]) / ray.dir.x * ray.dir.y, 2) + powf((ray.pos.x - wall[0]), 2));
	side_dist.y = sqrtf(powf((ray.pos.y - wall[1]) / ray.dir.y * ray.dir.x, 2) + powf((ray.pos.y - wall[1]), 2));
	res.dist = 0;
	while (get_map_char(&context->map, wall[0], wall[1]))
	{

	}

}

void	raycaster(t_context *context, int col)
{
	t_ray	ray;

	ray.pos = context->map.player.pos;
	ray.dir = (t_vec2){context->map.player.dir.x
		+ ((float)col / WIN_WIDTH * context->map.player.plane.x)
		- context->map.player.plane.x / 2,
		context->map.player.dir.y
		+ ((float)col / WIN_WIDTH * context->map.player.plane.y)
		- context->map.player.plane.y / 2
	};
	cast_ray(context, ray, col);
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
