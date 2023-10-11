/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_minimap.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:54:40 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:54:41 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw_line.h"

static void	draw_square(t_context *context, int x, int y)
{
	int		i;
	int		j;

	i = 0;
	while (i < context->minimap.mm_zoom * WIN_SIZE_PROPORTION)
	{
		j = 0;
		while (j < context->minimap.mm_zoom * WIN_SIZE_PROPORTION)
		{
			if (i == 0 || j == 0)
				set_img_pixel(&context->img, x + j, y + i, 0x0);
			else
				set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF
					- get_img_pixel(&context->img, x + j, y + i));
			j++;
		}
		i++;
	}
}

static void	render_walls(t_context *context)
{
	int				i;
	int				j;
	t_minimap const	minimap = context->minimap;
	t_player const	player = context->map.player;

	i = -minimap.max_search;
	while (i <= minimap.max_search)
	{
		j = -minimap.max_search;
		while (j <= minimap.max_search)
		{
			if (get_map_char(&(context->map), (int)player.pos.x + j,
					(int)player.pos.y + i) == '1')
				draw_square(context, minimap.center.x
					+ ((j + (int)player.pos.x - player.pos.x)
						* minimap.mm_zoom * WIN_SIZE_PROPORTION),
					minimap.center.y + ((i + (int)player.pos.y - player.pos.y)
						* minimap.mm_zoom * WIN_SIZE_PROPORTION));
			j++;
		}
		i++;
	}
}

static void	render_rays(t_context *context)
{
	int				i;
	t_point			start;
	t_point			stop;
	int const		color = context->minimap.ray_color;
	t_minimap const	minimap = context->minimap;

	start = (t_point){(int)minimap.center.x, (int)minimap.center.y};
	i = 0;
	while (i < context->win_width)
	{
		if (minimap.rays[i].wall.perceived_distance > 10)
			minimap.rays[i].wall.perceived_distance = 10;
		stop.x = start.x + (minimap.rays[i].ray.dir.x
				* minimap.rays[i].wall.perceived_distance * minimap.ray_factor
				* WIN_SIZE_PROPORTION);
		stop.y = start.y + (minimap.rays[i].ray.dir.y
				* minimap.rays[i].wall.perceived_distance * minimap.ray_factor
				* WIN_SIZE_PROPORTION);
		ft_draw_line_to_img(context, &start, &stop, color);
		i++;
	}
}

void	render_minimap(t_context *context)
{
	set_img_pixel(&context->img, context->minimap.center.x,
		context->minimap.center.y, 0XFFFFFF);
	render_rays(context);
	render_walls(context);
}
