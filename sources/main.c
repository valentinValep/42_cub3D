#include <stdio.h>
#include <X11/X.h>
#include <math.h>
#include <stdlib.h>
#include "cub3d.h"

int	init_context(t_context *context, char **argv)
{
	int	ret;

	context->mlx = mlx_init(); // @TODO check if mlx_init() failed
	if ((ret = init_map(context, argv[1])))
	{
		mlx_destroy_display(context->mlx);
		free(context->mlx);
		return (ret);
	}
	mlx_do_key_autorepeatoff(context->mlx);
	context->win = mlx_new_window(
		context->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE); // @TODO check if mlx_new_window() failed
	context->img.addr = mlx_new_image(
		context->mlx, WIN_WIDTH, WIN_HEIGHT); // @TODO check if mlx_new_image() failed
	context->img.pixels = mlx_get_data_addr(
		context->img.addr, &context->img.bpp,
		&context->img.line_len, &context->img.endian);
	context->img.width = WIN_WIDTH;
	context->img.height = WIN_HEIGHT;
	mlx_mouse_hide(context->mlx, context->win);
	mlx_mouse_move(context->mlx, context->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	for (int i = 0; i < KEY_NUMBER; i++)
		context->inputs[i] = 0;
	context->render_minimap = 0;
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

void	compute_key_pressed(t_context *context)
{
	const float	speed = SPEED * !(context->inputs[KEY_SHIFT_L])
		+ SPEED * context->inputs[KEY_SHIFT_L] * RUNNING_SPEED_MODIFIER;

	if (context->inputs[KEY_W])
	{
		context->map.player.speed.x = context->map.player.dir.x * speed;
		context->map.player.speed.y = context->map.player.dir.y * speed;
	}
	if (context->inputs[KEY_S])
	{
		context->map.player.speed.x = -context->map.player.dir.x * speed;
		context->map.player.speed.y = -context->map.player.dir.y * speed;
	}
	if (context->inputs[KEY_A])
	{
		context->map.player.speed.x = -context->map.player.plane.x * speed;
		context->map.player.speed.y = -context->map.player.plane.y * speed;
	}
	if (context->inputs[KEY_D])
	{
		context->map.player.speed.x = context->map.player.plane.x * speed;
		context->map.player.speed.y = context->map.player.plane.y * speed;
	}
	if (context->inputs[KEY_LEFT])
		context->map.player.rotate = -ROTATION_SPEED / 10;
	if (context->inputs[KEY_RIGHT])
		context->map.player.rotate = ROTATION_SPEED / 10;
}

void	compute_inputs(t_context *context)
{
	compute_key_pressed(context);
	if (context->map.player.speed.x != 0)
	{
		//if (get_map_char(&context->map,
		//		context->map.player.pos.x + context->map.player.speed.x,
		//		context->map.player.pos.y) != '1')
			context->map.player.pos.x += context->map.player.speed.x;
		context->map.player.speed.x = 0;
	}
	if (context->map.player.speed.y != 0)
	{
		//if (get_map_char(&context->map,
		//		context->map.player.pos.x,
		//		context->map.player.pos.y + context->map.player.speed.y) != '1')
			context->map.player.pos.y += context->map.player.speed.y;
		context->map.player.speed.y = 0;
	}
	if (context->map.player.rotate != 0)
	{
		rotate_player(context, context->map.player.rotate);
		context->map.player.rotate = 0;
		mlx_mouse_move(context->mlx, context->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	}
}

void	destroy_context(t_context *context)
{
	for (int i = 0; i < context->map.height; i++)
		free(context->map.grid[i]);
	free(context->map.grid);
	for (int i = 0; i < 4; i++)
		if (context->map.textures[i].addr)
			mlx_destroy_image(context->mlx, context->map.textures[i].addr);
	mlx_destroy_window(context->mlx, context->win);
	mlx_destroy_image(context->mlx, context->img.addr);
	mlx_destroy_display(context->mlx);
	free(context->mlx);
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
	render_main_scene(context);
	if (context->render_minimap)
		render_minimap(context);
	update(context);
	return (0);
}

int	main(int argc, char **argv)
{
	t_context	context;
	int			ret;

	if (argc != 2)
		return (basic_error("Usage: ./cub3d <map_path (.cub extension)>\n", 1));
	ret = init_context(&context, argv);
	if (ret)
		return (ret);
	mlx_hook(context.win, KeyPress, KeyPressMask,
		keydown_hook, &context);
	mlx_hook(context.win, KeyRelease, KeyReleaseMask,
		keyup_hook, &context);
	mlx_hook(context.win, DestroyNotify, NoEventMask,
		destroy_hook, &context);
	mlx_hook(context.win, MotionNotify, PointerMotionMask,
		motion_hook, &context);
	mlx_loop_hook(context.mlx, loop_hook, &context);
	//mlx_loop(context.mlx);
	destroy_context(&context);
}
