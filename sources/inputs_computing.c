#include "cub3d.h"

void	compute_fov(t_context *context)
{
	const float	actual_fov = get_player_fov(&context->map.player);

	if (context->inputs_handler.inputs[KEY_CTRL_L]
		|| context->inputs_handler.inputs[KEY_CTRL_R])
		return (change_player_fov(&context->map.player, ZOOM_FOV));
	if (context->map.player.running)
	{
		if (actual_fov < SPEED_FOV && actual_fov >= NORMAL_FOV)
			change_player_fov(
				&context->map.player, actual_fov + TRANSITION_FOV);
		else if (actual_fov < NORMAL_FOV)
			change_player_fov(&context->map.player, NORMAL_FOV);
		return ;
	}
	if (actual_fov > NORMAL_FOV)
		change_player_fov(&context->map.player, actual_fov - TRANSITION_FOV);
	else
		change_player_fov(&context->map.player, NORMAL_FOV);
}

static void	compute_minimap_zoom(t_context *context)
{
	if (context->map.player.running && context->minimap.max_search
		< MM_RUN_MAX_SEARCH && context->minimap.mm_zoom > MM_RUN_ZOOM)
	{
		context->minimap.max_search += TRANSITION_SEARCH;
		context->minimap.mm_zoom += TRANSITION_ZOOM;
		context->minimap.ray_factor += TRANSITION_FACTOR;
	}
	else if (!context->map.player.running && context->minimap.max_search
		> MM_MAX_SEARCH && context->minimap.mm_zoom < MM_ZOOM)
	{
		context->minimap.max_search -= TRANSITION_SEARCH;
		context->minimap.mm_zoom -= TRANSITION_ZOOM;
		context->minimap.ray_factor -= TRANSITION_FACTOR;
	}
	else if (!context->map.player.running && context->minimap.max_search
		< MM_MAX_SEARCH && context->minimap.mm_zoom > MM_ZOOM)
	{
		context->minimap.max_search = MM_MAX_SEARCH;
		context->minimap.mm_zoom = MM_ZOOM;
		context->minimap.ray_factor = TRANSITION_FACTOR;
	}
}

void	compute_key_pressed(t_context *context)
{
	float	speed;
	t_vec2	dir;

	dir = (t_vec2){!!context->inputs_handler.inputs[KEY_A]
		- !!context->inputs_handler.inputs[KEY_D],
		!!context->inputs_handler.inputs[KEY_W]
		- context->inputs_handler.inputs[KEY_S]};
	if (dir.x != 0 && dir.y != 0)
	{
		dir.x *= sqrtf(2) / 2;
		dir.y *= sqrtf(2) / 2;
	}
	context->map.player.running
		= (context->inputs_handler.inputs[KEY_SHIFT_L] && (dir.y > 0));
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
	compute_minimap_zoom(context);
}

static char	is_in_wall(t_context *context, float x, float y)
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
