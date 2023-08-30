#include "cub3d.h"
#include <math.h>

#define MAP_SCALE 10

void	render_player(t_context *context)
{
	const int	x = context->map.player.pos.x * MAP_SCALE
		+ WIN_WIDTH - context->map.width * MAP_SCALE;
	const int	y = context->map.player.pos.y * MAP_SCALE;

	set_img_pixel(&context->img, x, y, 0x0000FF);
	set_img_pixel(&context->img, x - 1, y - 1, 0x0000FF);
	set_img_pixel(&context->img, x - 1, y, 0x0000FF);
	set_img_pixel(&context->img, x - 1, y + 1, 0x0000FF);
	set_img_pixel(&context->img, x, y - 1, 0x0000FF);
	set_img_pixel(&context->img, x, y + 1, 0x0000FF);
	set_img_pixel(&context->img, x + 1, y - 1, 0x0000FF);
	set_img_pixel(&context->img, x + 1, y, 0x0000FF);
	set_img_pixel(&context->img, x + 1, y + 1, 0x0000FF);
}

void	render_minimap(t_context *context)
{
	char	c;

	for (int y = 0; y < context->map.height; y++)
		for (int x = 0; x < context->map.width; x++)
		{
			for (int k = 0; k < MAP_SCALE; k++)
				for (int l = 0; l < MAP_SCALE; l++)
				{
					c = get_map_char(&context->map, x, y);
					set_img_pixel(&context->img, x * MAP_SCALE + k + WIN_WIDTH - context->map.width * MAP_SCALE, y * MAP_SCALE + l, c == '1' ? 0xFF0000 : 0xFFFFFF);
				}
		}
	render_player(context);
}

float	compute_next_check_diff(float ray_dir, float ray_pos)
{
	if (ray_dir > 0)
		return (1 - (ray_pos - (int)ray_pos));
	else if (ray_dir < 0)
		return (ray_pos - (int)ray_pos);
	else
		return (1);
}
#include <stdio.h>
void	ray_caster(t_context *context, int col)
{
	const float		camera_x = 2 * col / (float)WIN_WIDTH - 1;
	const t_vec2	ray_dir = (t_vec2){
		context->map.player.dir.x + context->map.player.plane.x * camera_x,
		context->map.player.dir.y + context->map.player.plane.y * camera_x};
	t_vec2			ray_pos;
	t_vec2			next_check_diff;

	ray_pos = context->map.player.pos;
	while (get_map_char(&context->map, ray_pos.x, ray_pos.y) != '1')
	{
		next_check_diff = (t_vec2){
			compute_next_check_diff(ray_dir.x, ray_pos.x),
			compute_next_check_diff(ray_dir.y, ray_pos.y)};
		if (ray_dir.x / next_check_diff.x < ray_dir.y / next_check_diff.y)
		{
			ray_pos.x = roundf(ray_pos.x + next_check_diff.x);
			ray_pos.y += ray_dir.y / (next_check_diff.x / ray_dir.x);
		}
		else
		{
			ray_pos.x += ray_dir.x / (next_check_diff.y / ray_dir.y);
			ray_pos.y += roundf(ray_pos.y + next_check_diff.y);
		}
	}
	const float	wall_dist = sqrtf(powf(ray_pos.x - context->map.player.pos.x, 2) + powf(ray_pos.y - context->map.player.pos.y, 2)) * fabs(cosf(atan2f(ray_pos.y - context->map.player.pos.y, ray_pos.x - context->map.player.pos.x) - atan2f(context->map.player.dir.y, context->map.player.dir.x)));
	const int	wall_height = (int)(WALL_HEIGHT / wall_dist);
	const int	wall_start = WIN_HEIGHT / 2 - wall_height / 2;
	const int	wall_end = WIN_HEIGHT / 2 + wall_height / 2;
	for (int row = wall_start; row < wall_end; row++)
		set_img_pixel(&context->img, col, row, 0xFFFF00);
	set_img_pixel(&context->img, col, wall_start - 1, 0x000000);
	set_img_pixel(&context->img, col, wall_end + 1, 0x000000);
}

void	render_raycasting(t_context *context)
{
	for (int col = 0; col < WIN_WIDTH; col++)
	{
		for (int row = 0; row < WIN_HEIGHT; row++)
		{
			if (row < WIN_HEIGHT / 2)
				set_img_pixel(&context->img, col, row, 0xB2FFFF);
			else if (row == WIN_HEIGHT / 2)
				set_img_pixel(&context->img, col, row, 0xFF0000);
			else
				set_img_pixel(&context->img, col, row, 0x348C31);
		}
		ray_caster(context, col);
	}
}

void	render(t_context *context)
{
	render_raycasting(context);
	render_minimap(context);
}
