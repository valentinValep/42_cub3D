#include <math.h>
#include "cub3d.h"

float	invert(float n)
{
	if (n == 0)
		return (1e32);
	return (1 / n);
}

float	find_wall(t_map *map, t_ray *ray)
{
	t_map_square	wall;
	const t_vec2	delta_dist = {
		fabsf(invert(ray->dir.x)),
		fabsf(invert(ray->dir.y))};
	t_vec2			side_dist;
	float			res;

	wall = (t_map_square){(int)ray->pos.x, (int)ray->pos.y};
	side_dist.x = ((ray->dir.x > 0) - (ray->pos.x - wall.x)) * delta_dist.x;
	side_dist.y = ((ray->dir.y > 0) - (ray->pos.y - wall.y)) * delta_dist.y;
	while (get_map_char(map, wall.x, wall.y) != '1')
	{
		if (side_dist.x < side_dist.y)
		{
			wall.x += (ray->dir.x > 0) * 2 - 1;
			res = side_dist.x;
			side_dist.x += delta_dist.x;
		}
		else
		{
			wall.y += (ray->dir.y > 0) * 2 - 1;
			res = side_dist.y;
			side_dist.y += delta_dist.y;
		}
	}
	return (res);
}

void	render_wall(t_context *context, int col, float wall_dist)
{
	const float		line_height = WIN_HEIGHT / wall_dist;
	const int		draw_start = -line_height / 2 + WIN_HEIGHT / 2;
	const int		draw_end = line_height / 2 + WIN_HEIGHT / 2;

	for (int row = 0; row < WIN_HEIGHT; row++)
	{
		if (row < draw_start)
			set_img_pixel(&context->img, col, row, 0xB2FFFF);
		else if (row < draw_end)
			set_img_pixel(&context->img, col, row, 0xFFFF00);
		else
			set_img_pixel(&context->img, col, row, 0x348C31);
	}
}

void	ray_caster(t_context *context, int col)
{
	const float		camera_x = 2 * col / (float)WIN_WIDTH - 1;
	t_ray			ray;
	float			wall_dist;

	ray.dir = (t_vec2){
		context->map.player.dir.x + context->map.player.plane.x * camera_x,
		context->map.player.dir.y + context->map.player.plane.y * camera_x};
	ray.pos = context->map.player.pos;
	wall_dist = find_wall(&context->map, &ray);
	render_wall(context, col, wall_dist);
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
