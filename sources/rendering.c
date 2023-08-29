#include "cub3d.h"

#define MAP_SCALE 10

void	render_player(t_context *context)
{
	const int	x = context->map.player.x * MAP_SCALE
		+ WIN_WIDTH - context->map.width * MAP_SCALE;
	const int	y = context->map.player.y * MAP_SCALE;

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

void	render_raycasting(t_context *context)
{
	for (int col = 0; col < WIN_WIDTH; col++)
	{
		for (int row = 0; row < WIN_HEIGHT; row++)
		{
			if (row < WIN_HEIGHT / 2)
				set_img_pixel(&context->img, col, row, 0xB2FFFF);
			else
				set_img_pixel(&context->img, col, row, 0x348C31);
		}
	}
}

void	render(t_context *context)
{
	render_raycasting(context);
	render_minimap(context);
}
