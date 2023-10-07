/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:32:56 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/07 18:09:27 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_LINE_H
# define DRAW_LINE_H

/*
**	Includes
*/

# include "cub3d.h"

/*
**	Structures
*/

typedef struct s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct s_line_utils
{
	int		delta_x;
	int		delta_y;
	float	r;
}				t_line_utils;

/*
**		Line drawing functions' prototypes
**			Located in:	ft_draw_line_to_img.c and ft_draw_line_to_img_2.c
*/

void			ft_draw_line_to_img(
					t_context *context,
					t_point *start,
					t_point *stop,
					int color);
void			ft_draw_ver_line(
					t_context *context,
					t_point *start,
					t_point *stop,
					int color);
void			ft_draw_other_lines(
					t_context *context,
					t_point *start,
					t_point *stop,
					int color);
void			ft_draw_following_x(
					t_context *context,
					t_line_utils line,
					t_point start,
					int color);
void			ft_draw_following_y(
					t_context *context,
					t_line_utils line,
					t_point start,
					int color);
void			ft_negative_slope(
					t_context *context,
					t_line_utils line,
					t_point start,
					int color);
void			ft_positive_slope(
					t_context *context,
					t_line_utils line,
					t_point start,
					int color);

#endif
