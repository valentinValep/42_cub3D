#include <stdio.h>
#include "cub3D.h"

int	init_context(t_context *context, char **argv)
{
	int	ret;

	(void)argv;
	if ((ret = init_map(&context->map, argv[1])))
		return (ret);
	context->mlx = mlx_init(); // @TODO check if mlx_init() failed
	context->win = mlx_new_window(
		context->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE); // @TODO check if mlx_new_window() failed
	context->img.addr = mlx_new_image(
		context->mlx, WIN_WIDTH, WIN_HEIGHT); // @TODO check if mlx_new_image() failed
	context->img.pixels = mlx_get_data_addr(
		context->img.addr, &context->img.bpp,
		&context->img.line_len, &context->img.endian);
	return (0);
}

void	destroy_context(t_context *context)
{
	(void)context;
}

void	render(t_context *context)
{
	((int *) context->img.pixels)[0] = 0x00FFFFFF;
}

void	update(t_context *context)
{
	mlx_put_image_to_window(
		context->mlx, context->win, context->img.addr, 0, 0);
}

int	loop_hook(t_context *context)
{
	(void)context;
	render(context);
	update(context);
	return (0);
}

int	main(int argc, char **argv)
{
	t_context	context;
	int			ret;

	if (argc != 2)
		return (basic_error("Need one argument\n", 1));
	if ((ret = init_context(&context, argv)))
		return (ret);
	//mlx_hook(context.window, KeyPress, KeyPressMask,
	//	keydown_hook, &context);
	//mlx_hook(context.window, DestroyNotify, NoEventMask,
	//	destroy_hook, &context);
	mlx_loop_hook(context.mlx, loop_hook, &context);
	mlx_loop(context.mlx);
	destroy_context(&context);
}
