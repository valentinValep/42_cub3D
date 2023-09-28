#include "cub3d.h"

#define MAP_SCALE 3

void	render_player(t_context *context)
{
	const int	x = context->map.player.pos.x * MAP_SCALE
		+ context->win_width - context->map.width * MAP_SCALE;
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
	(void)context;

	for (int y = 0; y < context->map.height; y++)
		for (int x = 0; x < context->map.width; x++)
		{
			for (int k = 0; k < MAP_SCALE; k++)
				for (int l = 0; l < MAP_SCALE; l++)
				{
					c = is_wall_map(&context->map, x, y);
					set_img_pixel(
						&context->img,
						x * MAP_SCALE + k + context->win_width - context->map.width * MAP_SCALE,
						y * MAP_SCALE + l,
						c ? 0xFF0000 : 0xFFFFFF);
				}
		}
	render_player(context);
}
