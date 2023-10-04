#include "cub3d.h"

#include <stdio.h>

#define MAX_SEARCH	5
#define MM_ZOOM		25

static void	draw_square(t_context *context, int x, int y)
{
	if (context->inputs_handler.inputs[6]) // Shift_L
		context->minimap.mm_zoom = 14;
	for (int i = 0; i < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; i++)
		for (int j = 0; j < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; j++)
			if (i == 0 || j == 0)
				set_img_pixel(&context->img, x + j, y + i, 0x0);	//	Use other color ?
			else
				set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF);	//	Use other color ?
}

static void	render_walls(t_context *context)
{
	if (context->inputs_handler.inputs[6]) // Shift_L
	{
		context->minimap.mm_zoom = 14;
		context->minimap.max_search = 15;
	}
	// search walls surrounding player
	for (int i = -context->minimap.max_search; i <= context->minimap.max_search; i++)
		for (int j = -context->minimap.max_search; j <= context->minimap.max_search; j++)
			if (get_map_char(&(context->map), (int)context->map.player.pos.x + j, (int)context->map.player.pos.y + i) == '1')
				draw_square(context, (7 * (context->win_width / 8)) + ((j + (int)context->map.player.pos.x - context->map.player.pos.x) * context->minimap.mm_zoom * WIN_SIZE_PROPORTION), context->win_height - ((context->win_width / 8)) + ((i + (int)context->map.player.pos.y - context->map.player.pos.y) * context->minimap.mm_zoom * WIN_SIZE_PROPORTION)); // Magic number
}

void	render_minimap(t_context *context)
{
	context->minimap.max_search = MAX_SEARCH;
	context->minimap.mm_zoom = MM_ZOOM;
	//	calc minimap center position on screen.	//	Save in variable!
	set_img_pixel(&context->img, (7 * (context->win_width / 8))/* - MARGIN*/, context->win_height - ((context->win_width / 8))/* - MARGIN*/, 0XFFFFFF);	//	Test pixel
	//	render walls
		render_walls(context);
	//	render rays
		//for (int i = 0; i < 3; i++)
			//	draw minimap ray (with length depending on character speed / running)	//	need to import and adapt my line drawing function from fdf.
}
