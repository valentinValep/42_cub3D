/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:50:48 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:50:50 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdlib.h>

void	destroy_context(t_context *context)
{
	if (context->map.ready)
		destroy_map(&context->map, context->mlx);
	if (context->win)
	{
		mlx_mouse_show(context->mlx, context->win);
		mlx_destroy_window(context->mlx, context->win);
	}
	if (context->img.addr)
		mlx_destroy_image(context->mlx, context->img.addr);
	if (context->mlx)
	{
		mlx_do_key_autorepeaton(context->mlx);
		mlx_destroy_display(context->mlx);
		free(context->mlx);
	}
	if (context->minimap.rays)
		free(context->minimap.rays);
}
