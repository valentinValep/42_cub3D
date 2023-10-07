/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line_to_img.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 17:10:29 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/05 17:04:12 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	ft_swap_points(t_point **start, t_point **stop)
{
	t_point	*tmp;

	tmp = *start;
	*start = *stop;
	*stop = tmp;
}

void	ft_draw_ver_line(t_fdf fdf, t_point start, t_point stop, int color)
{
	int		delta_y;
	int		i;
	int		y;
	char	*tmp;

	tmp = fdf.img.img;
	y = 0;
	delta_y = stop.y - start.y;
	if (delta_y < 0)
		delta_y = -delta_y;
	tmp += start.x + (start.y * fdf.img.size_line);
	while (y < delta_y)
	{
		i = 0;
		while (i < 4)
		{
			if (tmp >= fdf.img.img && tmp < fdf.img.img_end)
				*tmp = (char)((color >> (i * 8)) & 0x000000FF);
			tmp++;
			i++;
		}
		tmp += fdf.img.size_line - 4;
		y++;
	}
}

void	ft_draw_following_x(
	t_fdf fdf, t_line_utils line, t_point start, int color)
{
	int		x;
	int		y;
	int		i;
	char	*tmp;

	tmp = fdf.img.img;
	x = 0;
	while (x <= line.delta_x)
	{
		y = (int)(line.r * (float)x);
		i = 0;
		while (i < 4)
		{
			if ((tmp + ((start.y + y) * fdf.img.size_line)
					+ ((start.x + x) * 4) + i) >= fdf.img.img
				&& (tmp + ((start.y + y) * fdf.img.size_line)
					+ ((start.x + x) * 4) + i) < fdf.img.img_end)
				*(tmp + ((start.y + y) * fdf.img.size_line)
						+ ((start.x + x) * 4) + i)
					= (char)((color >> (i * 8)) & 0x000000FF);
			i++;
		}
		x++;
	}
}

void	ft_draw_other_lines(
	t_fdf fdf, t_point *start, t_point *stop, int color)
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
		ft_draw_following_x(fdf, line, *start, color);
	else
		ft_draw_following_y(fdf, line, *start, color);
}

void	ft_draw_line_to_img(t_fdf fdf, t_point *start, t_point *stop, int color)	//	import t_point from fdf.h
{
	t_line_utils	line;	//	import from fdf.h

	line.delta_x = stop->x - start->x;
	if (line.delta_x > -1 && line.delta_x < 1)
		ft_draw_ver_line(fdf, *start, *stop, color);
	else
		ft_draw_other_lines(fdf, start, stop, color);
}
