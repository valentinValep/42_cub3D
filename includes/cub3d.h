#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include "libft.h"

# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define WIN_TITLE "Cub3D"
# define WALL_HEIGHT 64

typedef struct s_img {
	void	*addr;
	char	*pixels;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_vec2 {
	float	x;
	float	y;
}	t_vec2;

typedef struct s_player {
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
}	t_player;

typedef struct s_map {
	t_player	player;
	t_vector	grid; // vector<vector<char>>
	int			width;
	int			height;
	int			has_player;
}	t_map;

typedef struct s_context {
	void	*mlx;
	void	*win;
	t_img	img;
	t_map	map;
}	t_context;

// errors.c
int		basic_error(char *str, int ret);

// images.c
void	set_img_pixel(t_img *img, int x, int y, int color);
int		get_img_pixel(t_img *img, int x, int y);

// map.cs
int		init_map(t_map *map, char *path);
char	get_map_char(t_map *map, int x, int y);
void	set_map_char(t_map *map, int x, int y, char c);

// rendering.c
void	render(t_context *context);

#endif
