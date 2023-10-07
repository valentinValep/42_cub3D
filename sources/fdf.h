/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 16:32:56 by fguarrac          #+#    #+#             */
/*   Updated: 2022/10/25 15:26:41 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/errno.h>

# if !defined (HOME) && !defined (SCHOOL)
#  define SCHOOL
# endif

# ifdef SCHOOL
#  undef HOME
#  define TITTLEBAR 38
#  define MENUBAR 46
#  define WINX 2560
#  define WINY 1440
# endif

# ifdef HOME
#  define TITTLEBAR 29
#  define MENUBAR 45
#  define WINX 1280
#  define WINY 800
# endif

# ifdef TITTLEBARLEN
#  if TITTLEBARLEN != 176
#   undef TITTLEBARLEN
#  endif
# endif
# ifndef TITTLEBARLEN
#  define TITTLEBARLEN 176
# endif

# ifdef SPACE
#  if SPACE <= 0
#   undef SPACE
#  endif
# endif
# ifndef SPACE
#  define SPACE 30
# endif

# ifdef MARGIN
#  if MARGIN < 0
#   undef MARGIN
#  endif
#  if MARGIN > (WINX / 2)
#   undef MARGIN
#  endif
#  if MARGIN > (WINY / 2)
#   undef MARGIN
#  endif
#  if MARGIN > 100
#   undef MARGIN
#  endif
# endif
# ifndef MARGIN
#  define MARGIN 10
# endif

# ifdef FACTOR
#  if FACTOR <= 0
#   undef FACTOR
#  endif
# endif
# ifndef FACTOR
#  define FACTOR 4
# endif

# ifdef FORCE_FACTOR
#  if FORCE_FACTOR <= 0
#   undef FORCE_FACTOR
#  endif
#  undef FACTOR
#  define FACTOR 1
# endif
# ifndef FORCE_FACTOR
#  define FORCE_FACTOR 1
# endif

/*
**	Sructs & Typedefs
*/

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
}				t_mlx;

typedef struct s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct s_link
{
	int				tabsize;
	char			pad[4];
	int				*tab;
	struct s_link	*next;
	struct s_link	*prev;
}				t_link;

typedef struct s_img
{
	void	*mlx_img;
	char	*img;
	char	*img_end;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
	char	pad[4];
}				t_img;

typedef struct s_img_utils
{
	struct s_point	win;
	struct s_point	offset;
	double			factor;
	double			space;
}				t_img_utils;

typedef struct s_line_utils
{
	int		delta_x;
	int		delta_y;
	float	r;
}				t_line_utils;

typedef struct s_fdf
{
	struct s_mlx	mlx;
	struct s_link	*map;
	struct s_img	img;
	struct s_point	high;
	struct s_point	low;
	int				fd;
	int				high_value;
	int				low_value;
	int				max;
	int				min;
	int				max_x;
	int				columns;
	int				rows;
	int				useful;
	char			pad[4];
	char			*gnl_tmp;
}				t_fdf;

typedef t_point	t_offset;
typedef t_point	t_win;

/*
**		Structs initialisation function's prototype
**			Located in: init_structs.c
*/

void			ft_init_struct(t_fdf *fdf, t_img_utils *utils);

/*
**		Args check functions' prototypes
**			Located in: check_args.c
*/

void			ft_check_args(t_fdf *fdf, int argc, char **argv);
void			ft_check_argv(t_fdf *fdf, char **argv);

/*
**		Args error message functions' prototypes
**			Located in: arg_error.c and arg_error_2.c
*/

void			ft_arg_error(void)		__attribute__((noreturn));
void			ft_arg_error_2(void)	__attribute__((noreturn));
void			ft_arg_error_3(void)	__attribute__((noreturn));
void			ft_arg_error_4(void)	__attribute__((noreturn));
void			ft_arg_error_5(void);
void			ft_arg_error_6(void);
void			ft_arg_error_7(void);
void			ft_arg_error_8(void);

/*
**		Map parsing functions' prototypes
**			Located in: map_functions.c, map_functions_2.c and map_functions_3.c
*/

void			ft_get_map(t_fdf *fdf);
t_link			*ft_parse_map(t_fdf *fdf);
void			ft_parse_map_err(
					t_fdf *fdf, char *line, t_link *head)
				__attribute__((noreturn));
t_link			*ft_link(t_fdf *fdf, t_link **head);
void			ft_link_alloc(t_link **link);
void			ft_get_map_err_0(t_fdf *fdf, t_link *map);
void			ft_get_map_err_1(t_fdf *fdf, t_link *map);
void			ft_get_map_err_2(t_fdf *fdf, t_link *map);
int				ft_fill_uneven_map(t_fdf *fdf, t_link *map);
void			ft_fill_new_tab(t_fdf *fdf, t_link *map, int *tmp, int j);
void			ft_fill_new_tab_calc_peak(
					t_fdf *fdf, t_link *map, int i, int j);

/*
**		Get_next_line function prototype
**			Located in: get_next_line.c
*/

int				get_next_line(t_fdf *fdf, char **line);

/*
**		Ft_split_int functions' prototypes
**			Located in: ft_split_int.c and ft_split_int_2.c
*/

void			ft_split_int(t_fdf *fdf, char *str, t_link *link);
int				ft_fill_tab_peak(t_fdf *fdf, t_link *link, char *str);
int				ft_count_int(t_fdf *fdf, char *str);
int				ft_atoiptr(char *str, int *nbr, int *err);
int				ft_atoiptr_light(char *str, int *err);
void			ft_fill_calc_peaks(t_fdf *fdf, t_link *link, int nbr, int i);
void			ft_no_err(t_fdf *fdf, t_link *link, int nbr, int *i);

/*
**		Mlx init function's prototype
**			Located in: mlx_init.c 
*/

void			ft_mlx_init(t_fdf *fdf);

/*
**		Window / spacing / factor / peak calculation functions' protoypes
**			Located in:	calc_win.c, calc_factor_space.c,
**						ft_calc_factor_and_space.c, ft_calc_factor.c,
**						ft_calc_space and ft_update_peaks.c
*/

void			ft_calc_win(t_fdf *fdf, t_img_utils *utils);
void			ft_calc_factor_space(t_fdf *fdf, t_img_utils *utils);
void			ft_calc_factor_and_space(t_fdf *fdf, t_img_utils *utils);
void			ft_calc_factor(t_fdf *fdf, t_img_utils *utils);
void			ft_calc_space(t_fdf *fdf, t_img_utils *utils);
void			ft_adapt_win(t_fdf *fdf, t_img_utils *utils);
void			ft_update_peaks(t_fdf *fdf, t_img_utils *utils);

/*
**		Offset calculation function protoype
**			Located in:	ft_calc_offset.c
*/

void			ft_calc_offset(t_fdf *fdf, t_img_utils *utils);

/*
**		New window function prototype
**			Located in: get_window.c
*/

void			ft_get_new_win(t_fdf *fdf, t_img_utils utils);

/*
**		New image function prototype
**			Located in: get_image.c
*/

void			ft_get_new_img(t_fdf *fdf, t_img_utils utils);

/*
**		Map drawing functions' prototypes
**			Located in: draw_map.c
*/

void			ft_draw_map(t_fdf fdf, t_img_utils utils);
void			ft_calc_draw_hor(
					t_fdf fdf, t_img_utils utils, t_link *tmp, t_point coord);
void			ft_colorize_dot(t_fdf fdf, t_point start);
void			ft_calc_draw_ver(
					t_fdf fdf, t_img_utils utils, t_link *tmp, t_point coord);

/*
**		Line drawing functions' prototypes
**			Located in:	ft_draw_line_to_img.c and ft_draw_line_to_img_2.c
*/

void			ft_draw_line_to_img(
					t_fdf fdf, t_point *start, t_point *stop, int color);
void			ft_draw_ver_line(
					t_fdf fdf, t_point start, t_point stop, int color);
void			ft_draw_other_lines(
					t_fdf fdf, t_point *start, t_point *stop, int color);
void			ft_draw_following_x(
					t_fdf fdf, t_line_utils line, t_point start, int color);
void			ft_draw_following_y(
					t_fdf fdf, t_line_utils line, t_point start, int color);
void			ft_negative_slope(
					t_fdf fdf, t_line_utils line, t_point start, int color);
void			ft_positive_slope(
					t_fdf fdf, t_line_utils line, t_point start, int color);

/*
**		Cleaning functions' prototypes
**			Located in: cleaning_functions.c
*/

void			ft_free_map(t_link *map);
void			ft_close_fd(t_fdf *fdf);

/*
**		Mlx interrupts function's prototypes
**			Located in: hooks.c
*/

void			ft_set_hooks(t_fdf *fdf);
int				ft_key_release(int key, t_fdf *fdf);
int				ft_close_win(t_fdf *fdf) __attribute__((noreturn));

/*
**		Helper functions' prototypes
**			Located in: fdf_utils.c and ft_round.c
*/

int				ft_isspace(int c);
size_t			ft_strlen(const char *s);
char			*ft_strrchr(const char *s, int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
long long int	ft_round(double nbr);

#endif
