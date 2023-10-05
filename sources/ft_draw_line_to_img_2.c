/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line_to_img_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 02:17:56 by fguarrac          #+#    #+#             */
/*   Updated: 2022/04/06 17:39:42 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_positive_slope(
	t_fdf fdf, t_line_utils line, t_point start, int color)
{
	int				x;
	int				y;
	int				i;
	char			*tmp;

	tmp = fdf.img.img;
	y = 0;
	while (y < line.delta_y)
	{
		x = (int)((float)y / line.r);
		i = 0;
		while (i < 4)
		{
			if ((tmp + ((start.y + y) * fdf.img.size_line)
					+ ((start.x + x) * 4) + i)
				>= fdf.img.img && (tmp + ((start.y + y) * fdf.img.size_line)
					+ ((start.x + x) * 4) + i) < fdf.img.img_end)
				*(tmp + ((start.y + y) * fdf.img.size_line)
						+ ((start.x + x) * 4) + i)
					= (char)((color >> (i * 8)) & 0x000000FF);
			i++;
		}
		y++;
	}
}

void	ft_negative_slope(
	t_fdf fdf, t_line_utils line, t_point start, int color)
{
	int				x;
	int				y;
	int				i;
	char			*tmp;

	tmp = fdf.img.img;
	y = 0;
	while (y < line.delta_y)
	{
		x = (int)((float)y / line.r);
		i = 0;
		while (i < 4)
		{
			if ((tmp + ((start.y - y) * fdf.img.size_line)
					+ ((start.x - x) * 4) + i)
				>= fdf.img.img && (tmp + ((start.y - y) * fdf.img.size_line)
					+ ((start.x - x) * 4) + i) < fdf.img.img_end)
				*(tmp + ((start.y - y) * fdf.img.size_line)
						+ ((start.x - x) * 4) + i)
					= (char)((color >> (i * 8)) & 0x000000FF);
			i++;
		}
		y++;
	}
}

void	ft_draw_following_y(
	t_fdf fdf, t_line_utils line, t_point start, int color)
{
	if ((float)line.delta_y < 0.0f)
		line.delta_y = -line.delta_y;
	if (line.r < 0.0f)
		ft_negative_slope(fdf, line, start, color);
	else
		ft_positive_slope(fdf, line, start, color);
}
