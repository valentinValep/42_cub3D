#include "cub3d.h"

#define MAP_SCALE 10
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
					set_img_pixel(&context->img, x * MAP_SCALE + k + WIN_WIDTH - context->map.width * MAP_SCALE, y * MAP_SCALE + l, c == '1' ? 0xFF0000 : c == '0' ? 0xFFFFFF : 0x0000FF);
				}
		}
}

void	render(t_context *context)
{
	render_minimap(context);
}
