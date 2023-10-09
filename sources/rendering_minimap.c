#include "draw_line.h" // minimap.h

static void	draw_square(t_context *context, int x, int y)
{
	for (int i = 0; i < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; i++)
		for (int j = 0; j < context->minimap.mm_zoom * WIN_SIZE_PROPORTION; j++)
			if (i == 0 || j == 0)
				set_img_pixel(&context->img, x + j, y + i, 0x0);
			else
				set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF - get_img_pixel(&context->img, x + j, y + i));
}

static void	render_walls(t_context *context)
{
	t_minimap const	minimap = context->minimap;

	for (int i = -minimap.max_search; i <= minimap.max_search; i++)
		for (int j = -minimap.max_search; j <= minimap.max_search; j++)
			if (get_map_char(&(context->map), (int)context->map.player.pos.x + j, (int)context->map.player.pos.y + i) == '1')
				draw_square(context, minimap.center.x + ((j + (int)context->map.player.pos.x - context->map.player.pos.x) * minimap.mm_zoom * WIN_SIZE_PROPORTION), minimap.center.y + ((i + (int)context->map.player.pos.y - context->map.player.pos.y) * minimap.mm_zoom * WIN_SIZE_PROPORTION));
}

static void	render_rays(t_context *context)
{
	t_point			start;
	t_point			stop;
	int const		color = context->minimap.ray_color;
	t_minimap const	minimap = context->minimap;

	start = (t_point){(int)minimap.center.x, (int)minimap.center.y};
	for (int i = 0; i < context->win_width; i++)
	{
		if (minimap.rays[i].wall.perceived_distance > 10)	//	Draw all rays with same length?
			minimap.rays[i].wall.perceived_distance = 10;	//	Limit ray length
		stop.x = start.x + (minimap.rays[i].ray.dir.x * minimap.rays[i].wall.perceived_distance * minimap.ray_factor * WIN_SIZE_PROPORTION);
		stop.y = start.y + (minimap.rays[i].ray.dir.y * minimap.rays[i].wall.perceived_distance * minimap.ray_factor * WIN_SIZE_PROPORTION);
		ft_draw_line_to_img(context, &start, &stop, color);
	}
}

void	render_minimap(t_context *context)
{
	//	calc minimap center position on screen.
	set_img_pixel(&context->img, context->minimap.center.x, context->minimap.center.y, 0XFFFFFF);	//	Test pixel
	//	render rays
	render_rays(context);
	//	render walls
	render_walls(context);
}
