#include <stdio.h>
#include <X11/X.h>
#include <math.h>
#include "cub3d.h"

int	init_context(t_context *context, char **argv)
{
	int	ret;

	context->mlx = mlx_init(); // @TODO check if mlx_init() failed
	if ((ret = init_map(context, argv[1])))
		return (ret);
	context->win = mlx_new_window(
		context->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE); // @TODO check if mlx_new_window() failed
	context->img.addr = mlx_new_image(
		context->mlx, WIN_WIDTH, WIN_HEIGHT); // @TODO check if mlx_new_image() failed
	context->img.pixels = mlx_get_data_addr(
		context->img.addr, &context->img.bpp,
		&context->img.line_len, &context->img.endian);
	context->img.width = WIN_WIDTH;
	context->img.height = WIN_HEIGHT;
	return (0);
}

void	rotate_player(t_context *context, float angle)
{
	const float	old_dir_x = context->map.player.dir.x;
	const float	old_plane_x = context->map.player.plane.x;

	context->map.player.dir.x = context->map.player.dir.x * cos(angle)
		- context->map.player.dir.y * sin(angle);
	context->map.player.dir.y = old_dir_x * sin(angle)
		+ context->map.player.dir.y * cos(angle);
	context->map.player.plane.x = context->map.player.plane.x * cos(angle)
		- context->map.player.plane.y * sin(angle);
	context->map.player.plane.y = old_plane_x * sin(angle)
		+ context->map.player.plane.y * cos(angle);
}

void	compute_inputs(t_context *context)
{
	if (context->map.player.speed.x != 0)
	{
		if (get_map_char(&context->map,
				context->map.player.pos.x + context->map.player.speed.x,
				context->map.player.pos.y) != '1')
			context->map.player.pos.x += context->map.player.speed.x;
		context->map.player.speed.x = 0;
	}
	if (context->map.player.speed.y != 0)
	{
		if (get_map_char(&context->map,
				context->map.player.pos.x,
				context->map.player.pos.y + context->map.player.speed.y) != '1')
			context->map.player.pos.y += context->map.player.speed.y;
		context->map.player.speed.y = 0;
	}
	if (context->map.player.rotate != 0)
	{
		rotate_player(context, context->map.player.rotate);
		context->map.player.rotate = 0;
	}
}

void	destroy_context(t_context *context)
{
	(void)context;
}

void	update(t_context *context)
{
	mlx_put_image_to_window(
		context->mlx, context->win, context->img.addr, 0, 0);
}

int	loop_hook(t_context *context)
{
	(void)context;
	compute_inputs(context);
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
	mlx_hook(context.win, KeyPress, KeyPressMask,
		keydown_hook, &context);
	mlx_hook(context.win, DestroyNotify, NoEventMask,
		destroy_hook, &context);
	mlx_loop_hook(context.mlx, loop_hook, &context);
	mlx_loop(context.mlx);
	destroy_context(&context);
}
