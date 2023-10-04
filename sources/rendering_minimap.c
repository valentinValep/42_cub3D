#include "cub3d.h"

#include <stdio.h>

#define MAX_SEARCH	5
#define MM_ZOOM		25

static void	draw_square(t_context *context, int x, int y)
{
	for (int i = 0; i < MM_ZOOM * WIN_SIZE_PROPORTION/*running ? 3 : 5*/; i++)	//	Magic number
		for (int j = 0; j < MM_ZOOM * WIN_SIZE_PROPORTION; j++)
			set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF);
}

static void	search_walls(t_context *context)
{
	// search walls surrounding player
	for (int i = -MAX_SEARCH; i <= MAX_SEARCH; i++)
		for (int j = -MAX_SEARCH; j <= MAX_SEARCH; j++)
			if (get_map_char(&(context->map), (int)context->map.player.pos.x + j, (int)context->map.player.pos.y + i) == '1')
				draw_square(context, (7 * (context->win_width / 8)) + ((j + (int)context->map.player.pos.x - context->map.player.pos.x) * MM_ZOOM * WIN_SIZE_PROPORTION), context->win_height - ((context->win_width / 8)) + ((i + (int)context->map.player.pos.y - context->map.player.pos.y) * MM_ZOOM * WIN_SIZE_PROPORTION)); // Magic number
}

void	render_minimap(t_context *context)
{
	//	calc minimap center position on screen.	//	Save in variable!
	set_img_pixel(&context->img, (7 * (context->win_width / 8))/* - MARGIN*/, context->win_height - ((context->win_width / 8))/* - MARGIN*/, 0XFFFFFF);	//	Test pixel
	//	render walls
		search_walls(context);
	//	render rays
		//for (int i = 0; i < 3; i++)
			//	draw minimap ray (with length depending on character speed / running)	//	need to import and adapt my line drawing function from fdf.
}
