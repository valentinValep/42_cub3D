#include <stdio.h>
#include <X11/X.h>
#include <stdlib.h>
#include "cub3d.h"

int	init_context(t_context *context, char **argv)
{
	int	ret;
	int	screen_size[2];

	context->mlx = mlx_init(); // @TODO check if mlx_init() failed
	if ((ret = init_map(context, argv[1])))
	{
		mlx_destroy_display(context->mlx);
		free(context->mlx);
		return (ret);
	}
	mlx_do_key_autorepeatoff(context->mlx);
	mlx_get_screen_size(context->mlx, screen_size, screen_size + 1);
	context->win_width = screen_size[0] * WIN_SIZE_PROPORTION;
	context->win_height = screen_size[1] * WIN_SIZE_PROPORTION;
	context->win = mlx_new_window(
		context->mlx, context->win_width, context->win_height, WIN_TITLE); // @TODO check if mlx_new_window() failed
	context->img.addr = mlx_new_image(
		context->mlx, context->win_width, context->win_height); // @TODO check if mlx_new_image() failed
	context->img.pixels = mlx_get_data_addr(
		context->img.addr, &context->img.bpp,
		&context->img.line_len, &context->img.endian);
	context->img.width = context->win_width;
	context->img.height = context->win_height;
	mlx_mouse_hide(context->mlx, context->win);
	mlx_mouse_move(context->mlx, context->win, context->win_width / 2, context->win_height / 2);
	for (int i = 0; i < KEY_NUMBER; i++)
		context->inputs_handler.inputs[i] = 0;
	context->inputs_handler.render_minimap = 0;
	context->inputs_handler.active_collisions = 0;
	return (0);
}

void	compute_fov(t_context *context)
{
	const float	actual_fov = get_player_fov(&context->map.player);

	if (context->inputs_handler.inputs[KEY_CTRL_L])
		return (change_player_fov(&context->map.player, ZOOM_FOV));
	if (context->map.player.running)
	{
		if (actual_fov < SPEED_FOV && actual_fov >= NORMAL_FOV)
			change_player_fov(&context->map.player, actual_fov + TRANSITION_FOV);
		else if (actual_fov < NORMAL_FOV)
			change_player_fov(&context->map.player, NORMAL_FOV);
		return ;
	}
	if (actual_fov > NORMAL_FOV)
		change_player_fov(&context->map.player, actual_fov - TRANSITION_FOV);
	else
		change_player_fov(&context->map.player, NORMAL_FOV);
}

void	compute_key_pressed(t_context *context)
{
	float	speed;
	t_vec2	dir;

	dir = (t_vec2){!!context->inputs_handler.inputs[KEY_A] - !!context->inputs_handler.inputs[KEY_D],
		!!context->inputs_handler.inputs[KEY_W] - context->inputs_handler.inputs[KEY_S]};
	if (dir.x != 0 && dir.y != 0)
	{
		dir.x *= sqrt(2) / 2;
		dir.y *= sqrt(2) / 2;
	}
	context->map.player.running = context->inputs_handler.inputs[KEY_SHIFT_L]
		&& (dir.y > 0);
	speed = SPEED * (1 + (context->map.player.running * (RUNNING_MODIFIER -1)));
	context->map.player.speed.x = context->map.player.dir.x * dir.y * speed
		+ context->map.player.dir.y * dir.x * speed;
	context->map.player.speed.y = context->map.player.dir.y * dir.y * speed
		- context->map.player.dir.x * dir.x * speed;
	if (context->inputs_handler.inputs[KEY_LEFT])
		context->map.player.rotate = -ROTATION_SPEED / 10;
	if (context->inputs_handler.inputs[KEY_RIGHT])
		context->map.player.rotate = ROTATION_SPEED / 10;
	compute_fov(context);
}

char	is_in_wall(t_context *context, float x, float y)
{
	return (is_wall_map(&context->map, floorf(x + COLLISION_BOX_SIZE),
			floorf(y + COLLISION_BOX_SIZE))
		|| is_wall_map(&context->map, floorf(x + COLLISION_BOX_SIZE),
			floorf(y - COLLISION_BOX_SIZE))
		|| is_wall_map(&context->map, floorf(x - COLLISION_BOX_SIZE),
			floorf(y + COLLISION_BOX_SIZE))
		|| is_wall_map(&context->map, floorf(x - COLLISION_BOX_SIZE),
			floorf(y - COLLISION_BOX_SIZE))
	);
}

void	compute_inputs(t_context *context)
{
	compute_key_pressed(context);
	if (context->map.player.speed.x != 0)
	{
		if (!context->inputs_handler.active_collisions
			|| !is_in_wall(context,
				context->map.player.pos.x + context->map.player.speed.x,
				context->map.player.pos.y))
			context->map.player.pos.x += context->map.player.speed.x;
		context->map.player.speed.x = 0;
	}
	if (context->map.player.speed.y != 0)
	{
		if (!context->inputs_handler.active_collisions
			|| !is_in_wall(context, context->map.player.pos.x,
				context->map.player.pos.y + context->map.player.speed.y))
			context->map.player.pos.y += context->map.player.speed.y;
		context->map.player.speed.y = 0;
	}
	if (context->map.player.rotate != 0)
	{
		rotate_player(context, context->map.player.rotate);
		context->map.player.rotate = 0;
		mlx_mouse_move(context->mlx, context->win,
			context->win_width / 2, context->win_height / 2);
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
	if (context->inputs_handler.render_minimap)
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
	mlx_loop(context.mlx);
	mlx_do_key_autorepeaton(context.mlx);
	destroy_context(&context);
}
