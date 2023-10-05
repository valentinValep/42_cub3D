#include "cub3d.h"

#include <stdio.h>

static void	draw_square(t_context *context, int x, int y)
{
//	if (context->inputs_handler.inputs[KEY_SHIFT_L] && context->map.player.running)
//		context->minimap.mm_zoom = MM_RUN_ZOOM;
	for (int i = 0; i < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; i++)
		for (int j = 0; j < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; j++)
			if (i == 0 || j == 0)
				set_img_pixel(&context->img, x + j, y + i, 0x0);
			else
				set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF);
}

static void	render_walls(t_context *context)
{
//	if (context->inputs_handler.inputs[KEY_SHIFT_L] & context->map.player.running)
//	{
//		context->minimap.max_search = MM_RUN_MAX_SEARCH;
//		context->minimap.mm_zoom = MM_RUN_ZOOM;
//	}
	// search walls surrounding player
	for (int i = -context->minimap.max_search; i <= context->minimap.max_search; i++)
		for (int j = -context->minimap.max_search; j <= context->minimap.max_search; j++)
			if (get_map_char(&(context->map), (int)context->map.player.pos.x + j, (int)context->map.player.pos.y + i) == '1')
				draw_square(context, context->minimap.center.x + ((j + (int)context->map.player.pos.x - context->map.player.pos.x) * context->minimap.mm_zoom * WIN_SIZE_PROPORTION), context->minimap.center.y + ((i + (int)context->map.player.pos.y - context->map.player.pos.y) * context->minimap.mm_zoom * WIN_SIZE_PROPORTION)); // Magic number
}

void	render_minimap(t_context *context)
{
	//	calc minimap center position on screen.	//	Save in variable!
	set_img_pixel(&context->img, context->minimap.center.x, context->minimap.center.y, 0XFFFFFF);	//	Test pixel
	//	render walls
		render_walls(context);
	//	render rays
		//for (int i = 0; i < 3; i++)
			//	draw minimap ray (with length depending on character speed / running)	//	need to import and adapt my line drawing function from fdf.
}
