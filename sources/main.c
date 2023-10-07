#include <stdio.h>
#include <X11/X.h>
#include <stdlib.h>
#include "cub3d.h"

void	update(t_context *context)
{
	mlx_put_image_to_window(
		context->mlx, context->win, context->img.addr, 0, 0);
}

int	loop_hook(t_context *context)
{
	(void)context;
	compute_inputs(context);
	render_main_scene(context);
	if (context->inputs_handler.render_minimap)
		render_minimap(context);
	update(context);
	return (0);
}

int	main(int argc, char **argv)
{
	t_context	context;

	if (argc != 2)
		return (print_error("Usage: ./cub3d <map_path (.cub extension)>\n")
			, EXIT_FAILURE);
	if (init_context(&context, argv))
		return (destroy_context(&context), EXIT_FAILURE);
	mlx_hook(context.win, KeyPress, KeyPressMask,
		keydown_hook, &context);
	mlx_hook(context.win, KeyRelease, KeyReleaseMask,
		keyup_hook, &context);
	mlx_hook(context.win, DestroyNotify, NoEventMask,
		destroy_hook, &context);
	mlx_hook(context.win, MotionNotify, PointerMotionMask,
		motion_hook, &context);
	mlx_loop_hook(context.mlx, loop_hook, &context);
	mlx_loop(context.mlx);
	mlx_do_key_autorepeaton(context.mlx);
	mlx_mouse_show(context.mlx, context.win);
	destroy_context(&context);
}
