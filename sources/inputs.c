#include <X11/keysym.h>
#include "cub3d.h"

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
		context->map.player.speed.x = context->map.player.dir.x * 0.1;
		context->map.player.speed.y = context->map.player.dir.y * 0.1;
	}
	if (keycode == XK_s)
	{
		context->map.player.speed.x = -context->map.player.dir.x * 0.1;
		context->map.player.speed.y = -context->map.player.dir.y * 0.1;
	}
	if (keycode == XK_a)
	{
		context->map.player.speed.x = -context->map.player.plane.x * 0.1;
		context->map.player.speed.y = -context->map.player.plane.y * 0.1;
	}
	if (keycode == XK_d)
	{
		context->map.player.speed.x = context->map.player.plane.x * 0.1;
		context->map.player.speed.y = context->map.player.plane.y * 0.1;
	}
	if (keycode == XK_Left)
		context->map.player.rotate = -0.1;
	if (keycode == XK_Right)
		context->map.player.rotate = 0.1;
	return (0);
}
