/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:52:26 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:52:28 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/keysym.h>
#include "cub3d.h"

int	destroy_hook(t_context *context)
{
	mlx_loop_end(context->mlx);
	return (0);
}

int	keydown_hook(int keycode, t_context *context)
{
	const int	inputs[] = {XK_w, XK_s, XK_a, XK_d, XK_Left,
		XK_Right, XK_Shift_L, XK_Control_L, XK_Control_R};
	int			i;

	if (keycode == XK_Escape)
		mlx_loop_end(context->mlx);
	i = 0;
	while (i < KEY_NUMBER)
	{
		if (inputs[i] == keycode)
			context->inputs_handler.inputs[i] = 1;
		i++;
	}
	if (keycode == XK_m)
		context->inputs_handler.render_minimap
			= !context->inputs_handler.render_minimap;
	if (keycode == XK_k)
		context->inputs_handler.active_collisions
			= !context->inputs_handler.active_collisions;
	return (0);
}

int	keyup_hook(int keycode, t_context *context)
{
	const int	inputs[] = {XK_w, XK_s, XK_a, XK_d, XK_Left,
		XK_Right, XK_Shift_L, XK_Control_L, XK_Control_R};
	int			i;

	i = 0;
	while (i < KEY_NUMBER)
	{
		if (inputs[i] == keycode)
			context->inputs_handler.inputs[i] = 0;
		i++;
	}
	return (0);
}

int	motion_hook(int x, int y, t_context *context)
{
	(void)y;
	context->map.player.rotate
		= (x - context->win_width / 2) * ROTATION_SPEED / 100;
	return (0);
}
