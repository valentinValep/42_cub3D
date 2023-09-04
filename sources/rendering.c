#include <math.h>
#include "cub3d.h"

float	invert(float n)
{
	if (n == 0)
		return (1e32);
	return (1 / n);
}
#include <stdio.h>
t_nearest_wall	find_nearest_wall(t_map *map, t_ray *ray)
{
	t_map_square	wall;
	const t_vec2	delta_dist = {
		fabsf(invert(ray->dir.x)),
		fabsf(invert(ray->dir.y))};
	t_vec2			side_dist;
	t_nearest_wall	res;

	wall = (t_map_square){(int)ray->pos.x, (int)ray->pos.y};
	side_dist.x = fabsf((ray->dir.x > 0) - (ray->pos.x - wall.x)) * delta_dist.x;
	side_dist.y = fabsf((ray->dir.y > 0) - (ray->pos.y - wall.y)) * delta_dist.y;
	while (get_map_char(map, wall.x, wall.y) != '1')
	{
		if (side_dist.x < side_dist.y)
		{
			wall.x += (ray->dir.x > 0) * 2 - 1;
			res.dist = side_dist.x;
			res.side = WEST * (ray->dir.x < 0) + EAST * (ray->dir.x > 0);;
			side_dist.x += delta_dist.x;
		}
		else
		{
			wall.y += (ray->dir.y > 0) * 2 - 1;
			res.dist = side_dist.y;
			res.side = NORTH * (ray->dir.y < 0) + SOUTH * (ray->dir.y > 0);;
			side_dist.y += delta_dist.y;
		}
	}
	return (res);
}

int	get_wall_color(int side)
{
	if (side == NORTH)
		return (0x2222BB);
	if (side == SOUTH)
		return (0xFF2222);
	if (side == WEST)
		return (0xBBBB22);
	if (side == EAST)
		return (0x22BB22);
	return (0);
}

void	render_wall(t_context *context, int col, t_nearest_wall nearest_wall)
{
	const float		line_height = WIN_HEIGHT / nearest_wall.dist;
	const int		draw_start = -line_height / 2 + WIN_HEIGHT / 2;
	const int		draw_end = line_height / 2 + WIN_HEIGHT / 2;
	const int		color = get_wall_color(nearest_wall.side);

	for (int row = 0; row < WIN_HEIGHT; row++)
	{
		if (row < draw_start)
			set_img_pixel(&context->img, col, row, 0xB2FFFF);
		else if (row < draw_end)
			set_img_pixel(&context->img, col, row, color);
		else
			set_img_pixel(&context->img, col, row, 0x348C31);
	}
}
#include <stdio.h>
void	ray_caster(t_context *context, int col)
{
	const float		camera_x = 2 * col / (float)WIN_WIDTH - 1;
	t_ray			ray;
	t_nearest_wall	nearest_wall;

	ray.dir = (t_vec2){
		context->map.player.dir.x + context->map.player.plane.x * camera_x,
		context->map.player.dir.y + context->map.player.plane.y * camera_x};
	ray.pos = context->map.player.pos;
	nearest_wall = find_nearest_wall(&context->map, &ray);
	render_wall(context, col, nearest_wall);
}

void	render_main_scene(t_context *context)
{
	for (int col = 0; col < WIN_WIDTH; col++)
		ray_caster(context, col);
}

void	render(t_context *context)
{
	render_main_scene(context);
	render_minimap(context);
}
