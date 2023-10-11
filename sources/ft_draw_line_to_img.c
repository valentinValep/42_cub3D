/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line_to_img.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 17:10:29 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:14:03 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw_line.h"

static void	ft_swap_points(t_point **start, t_point **stop)
{
	t_point	*tmp;

	tmp = *start;
	*start = *stop;
	*stop = tmp;
}

void	ft_draw_ver_line(
	t_context *context, t_point *start, t_point *stop, int color)
{
	int		delta_y;
	int		y;

	y = 0;
	delta_y = stop->y - start->y;
	if (delta_y < 0)
	{
		delta_y = -delta_y;
		ft_swap_points(&start, &stop);
	}
	while (y < delta_y)
	{
		set_img_pixel(&(context->img), start->x, start->y + y, color);
		y++;
	}
}

void	ft_draw_following_x(
	t_context *context, t_line_utils line, t_point start, int color)
{
	int		x;
	int		y;

	x = 0;
	while (x <= line.delta_x)
	{
		y = (int)(line.r * (float)x);
		set_img_pixel(&(context->img), start.x + x, start.y + y, color);
		x++;
	}
}

void	ft_draw_other_lines(
	t_context *context, t_point *start, t_point *stop, int color)
{
	t_line_utils	line;

	line.delta_x = stop->x - start->x;
	if (line.delta_x < 0)
	{
		ft_swap_points(&start, &stop);
		line.delta_x = stop->x - start->x;
	}
	line.delta_y = stop->y - start->y;
	line.r = ((float)line.delta_y / (float)line.delta_x);
	if (line.r < 1.0f && line.r > -1.0f)
		ft_draw_following_x(context, line, *start, color);
	else
		ft_draw_following_y(context, line, *start, color);
}

void	ft_draw_line_to_img(
	t_context *context, t_point *start, t_point *stop, int color)
{
	t_line_utils	line;

	line.delta_x = stop->x - start->x;
	if (line.delta_x > -1 && line.delta_x < 1)
		ft_draw_ver_line(context, start, stop, color);
	else
		ft_draw_other_lines(context, start, stop, color);
}
