#include "cub3d.h"
#include <stdio.h>

void	draw_col(t_context *context, t_nearest_wall wall, t_ray ray, int col);

void	next_wall(t_ray ray, t_vec2 *side_dist, t_nearest_wall *res, int *wall)
{
	if (side_dist->x < side_dist->y)
	{
		wall[0] += (ray.dir.x > 0) * 2 - 1;
		res->perceived_distance = side_dist->x;
		res->side = EAST * (ray.dir.x < 0) + WEST * !(ray.dir.x < 0);
		side_dist->x += ray.delta[0];
	}
	else
	{
		wall[1] += (ray.dir.y > 0) * 2 - 1;
		res->perceived_distance = side_dist->y;
		res->side = SOUTH * (ray.dir.y < 0) + NORTH * !(ray.dir.y < 0);
		side_dist->y += ray.delta[1];
	}
}

t_nearest_wall	cast_ray(t_context *context, t_ray ray)
{
	t_vec2			side_dist;
	t_nearest_wall	res;
	int				*wall;
	int				step;

	step = 0;
	wall = (int [2]){floorf(ray.pos.x), floorf(ray.pos.y)};
	side_dist.x = fabs((ray.dir.x > 0) - (ray.pos.x - wall[0])) * ray.delta[0];
	side_dist.y = fabs((ray.dir.y > 0) - (ray.pos.y - wall[1])) * ray.delta[1];
	res.perceived_distance = 0;
	res.side = 0;
	while (!is_wall_map(&context->map, wall[0], wall[1])
		&& step < RENDER_DISTANCE)
	{
		next_wall(ray, &side_dist, &res, wall);
		step++;
	}
	if (step == RENDER_DISTANCE)
		res.perceived_distance = INFINITY;
	return (res);
}

void	raycaster(t_context *context, int col)
{
	t_ray			ray;
	t_nearest_wall	nearest_wall;

	ray.pos = context->map.player.pos;
	ray.dir = (t_vec2){context->map.player.dir.x * context->map.player.fov
		+ ((float)col / context->win_width * context->map.player.plane.x)
		- context->map.player.plane.x / 2,
		context->map.player.dir.y * context->map.player.fov
		+ ((float)col / context->win_width * context->map.player.plane.y)
		- context->map.player.plane.y / 2
	};
	ray.delta[0] = fabs(1 / ray.dir.x);
	ray.delta[1] = fabs(1 / ray.dir.y);
	nearest_wall = cast_ray(context, ray);
//	if (col == 0)
//	{
//		context->minimap.rays[0].ray = ray;
//		context->minimap.rays[0].wall = nearest_wall;	//	only take distance ?
//	}
	//if (!(col % (context->win_width / MM_RAYS)))	//	Save data for minimap to avoid computing the data twice.
	{
		//dprintf(1, "%d\n", col);
		//context->minimap.rays[(int)floor(col / (context->win_width / MM_RAYS))].ray = ray;
		//context->minimap.rays[(int)floor(col / (context->win_width / MM_RAYS))].wall = nearest_wall;	//	only take distance ?
		context->minimap.rays[col].ray = ray;
		context->minimap.rays[col].wall = nearest_wall;	//	only take distance ?
	}
	draw_col(context, nearest_wall, ray, col);
}

void	render_main_scene(t_context *context)
{
	int	col;

	col = 0;
	while (col < context->win_width)
	{
		raycaster(context, col);
		col++;
	}
}
