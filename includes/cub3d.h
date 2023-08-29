#ifndef CUB3D_H
# define CUB3D_H

# include "mlx.h"
# include "libft.h"

# define WIN_WIDTH 800
# define WIN_HEIGHT 600
# define WIN_TITLE "Cub3D"

typedef struct s_img {
	void	*addr;
	char	*pixels;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_map {
	int			width;
	int			height;
	int			has_player;
	t_vector	grid; // vector<vector<char>>
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

// rendering.c
void	render(t_context *context);

#endif
