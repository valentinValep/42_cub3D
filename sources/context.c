#include <stdlib.h>
#include "cub3d.h"

void	destroy_context(t_context *context)
{
	if (context->map.ready)
		destroy_map(&context->map, context->mlx);
	if (context->win)
		mlx_destroy_window(context->mlx, context->win);
	if (context->img.addr)
		mlx_destroy_image(context->mlx, context->img.addr);
	if (context->mlx)
	{
		mlx_destroy_display(context->mlx);
		free(context->mlx);
	}
	if (context->minimap.rays)
		free(context->minimap.rays);
}

static void	first_init(t_context *context)
{
	context->mlx = NULL;
	context->win = NULL;
	context->img.addr = NULL;
	context->map.ready = FALSE;
}

static int	create_window(t_context *context)
{
	int	screen_size[2];

	mlx_get_screen_size(context->mlx, screen_size, screen_size + 1);
	context->win_width = screen_size[0] * WIN_SIZE_PROPORTION;
	context->win_height = screen_size[1] * WIN_SIZE_PROPORTION;
	context->win = mlx_new_window(
			context->mlx, context->win_width, context->win_height, WIN_TITLE);
	return (!!context->win);
}

static void	init_inputs(t_context *context)
{
	int	i;

	i = 0;
	mlx_mouse_hide(context->mlx, context->win);
	mlx_mouse_move(
		context->mlx, context->win,
		context->win_width / 2, context->win_height / 2);
	while (i < KEY_NUMBER)
		context->inputs_handler.inputs[i++] = 0;
	context->inputs_handler.render_minimap = FALSE;
	context->inputs_handler.active_collisions = FALSE;
}

static void	init_minimap(t_context *context)
{
	context->minimap.max_search = MM_MAX_SEARCH;
	context->minimap.mm_zoom = MM_ZOOM;
	context->minimap.center.x = (15 * (context->win_width / 16));
	context->minimap.center.y
		= context->win_height - ((context->win_width / 16));
	context->minimap.rays = (t_mm_rays *)malloc(sizeof(t_mm_rays) * context->win_width);
	if (!(context->minimap.rays))
	{
		//	print Error + specific message + clean everything + return
		return ;
	}
	context->minimap.ray_color = 0x00FFFFFF - context->map.ground_color;
	context->minimap.ray_factor = MM_RAY_FACTOR;
}

int	init_context(t_context *context, char **argv)
{
	first_init(context);
	context->mlx = mlx_init();
	if (!context->mlx)
		return (print_error("Failed to init mlx\n"), EXIT_FAILURE);
	if (init_map(context, argv[1]))
		return (EXIT_FAILURE);
	context->map.ready = TRUE;
	mlx_do_key_autorepeatoff(context->mlx);
	if (!create_window(context))
		return (print_error("Failed to create window\n"), EXIT_FAILURE);
	context->img.addr = mlx_new_image(
			context->mlx, context->win_width, context->win_height);
	if (!context->img.addr)
		return (print_error("Failed to create image\n"), EXIT_FAILURE);
	context->img.pixels = mlx_get_data_addr(
			context->img.addr, &context->img.bpp,
			&context->img.line_len, &context->img.endian);
	context->img.width = context->win_width;
	context->img.height = context->win_height;
	init_inputs(context);
	init_minimap(context);
	return (0);
}
