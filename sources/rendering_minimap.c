#include "draw_line.h" // minimap.h

static void	draw_square(t_context *context, int x, int y)
{
	for (int i = 0; i < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; i++)
		for (int j = 0; j < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; j++)
			if (i == 0 || j == 0)
				set_img_pixel(&context->img, x + j, y + i, 0x0);
			else
				set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF);
}

static void	render_walls(t_context *context)
{
	// search walls surrounding player
	for (int i = -context->minimap.max_search; i <= context->minimap.max_search; i++)
		for (int j = -context->minimap.max_search; j <= context->minimap.max_search; j++)
			if (get_map_char(&(context->map), (int)context->map.player.pos.x + j, (int)context->map.player.pos.y + i) == '1')
				draw_square(context, context->minimap.center.x + ((j + (int)context->map.player.pos.x - context->map.player.pos.x) * context->minimap.mm_zoom * WIN_SIZE_PROPORTION), context->minimap.center.y + ((i + (int)context->map.player.pos.y - context->map.player.pos.y) * context->minimap.mm_zoom * WIN_SIZE_PROPORTION));
}

static void	render_rays(t_context *context)
{
	t_point 	start;
	t_point		stop;
	int const	color = context->minimap.ray_color;

	start = (t_point){(int)context->minimap.center.x, (int)context->minimap.center.y};
	for (int i = 0; i < MM_RAYS; i++)
	{
		//	draw minimap ray (with length depending on character speed / running)	//	need to import and adapt my line drawing function from fdf.
		stop.x = start.x + (context->minimap.rays[i].ray.dir.x * context->minimap.rays[i].wall.perceived_distance * 10 * WIN_SIZE_PROPORTION);	//	Magic number ! Need to change with acceleration !
		stop.y = start.y + (context->minimap.rays[i].ray.dir.y * context->minimap.rays[i].wall.perceived_distance * 10 * WIN_SIZE_PROPORTION);
		ft_draw_line_to_img(context, &start, &stop, color);
	}
}

void	render_minimap(t_context *context)
{
	//	calc minimap center position on screen.
	set_img_pixel(&context->img, context->minimap.center.x, context->minimap.center.y, 0XFFFFFF);	//	Test pixel
	//	render walls
	render_walls(context);
	//	render rays
	render_rays(context);
}
