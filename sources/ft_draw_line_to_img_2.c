/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line_to_img_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 02:17:56 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:12:44 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw_line.h"

void	ft_positive_slope(
	t_context *context, t_line_utils line, t_point start, int color)
{
	int				x;
	int				y;

	y = 0;
	while (y < line.delta_y)
	{
		x = (int)((float)y / line.r);
		set_img_pixel(&(context->img), start.x + x, start.y + y, color);
		y++;
	}
}

void	ft_negative_slope(
	t_context *context, t_line_utils line, t_point start, int color)
{
	int				x;
	int				y;

	y = 0;
	while (y < line.delta_y)
	{
		x = (int)((float)y / line.r);
		set_img_pixel(&(context->img), start.x - x, start.y - y, color);
		y++;
	}
}

void	ft_draw_following_y(
	t_context *context, t_line_utils line, t_point start, int color)
{
	if ((float)line.delta_y < 0.0f)
		line.delta_y = -line.delta_y;
	if (line.r < 0.0f)
		ft_negative_slope(context, line, start, color);
	else
		ft_positive_slope(context, line, start, color);
}
