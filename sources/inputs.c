#include <X11/keysym.h>
#include "cub3d.h"

#define SPEED 0.3
#define ROTATION_SPEED 0.2

int	destroy_hook(t_context *context)
{
	mlx_loop_end(context->mlx);
	return (0);
}

int	keydown_hook(int keycode, t_context *context)
{
	if (keycode == XK_Escape)
		mlx_loop_end(context->mlx);
	if (keycode == XK_w)
	{
		context->map.player.speed.x = context->map.player.dir.x * SPEED;
		context->map.player.speed.y = context->map.player.dir.y * SPEED;
	}
	if (keycode == XK_s)
	{
		context->map.player.speed.x = -context->map.player.dir.x * SPEED;
		context->map.player.speed.y = -context->map.player.dir.y * SPEED;
	}
	if (keycode == XK_a)
	{
		context->map.player.speed.x = -context->map.player.plane.x * SPEED;
		context->map.player.speed.y = -context->map.player.plane.y * SPEED;
	}
	if (keycode == XK_d)
	{
		context->map.player.speed.x = context->map.player.plane.x * SPEED;
		context->map.player.speed.y = context->map.player.plane.y * SPEED;
	}
	if (keycode == XK_Left)
		context->map.player.rotate = -ROTATION_SPEED;
	if (keycode == XK_Right)
		context->map.player.rotate = ROTATION_SPEED;
	return (0);
}
