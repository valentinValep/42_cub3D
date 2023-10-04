#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include "mlx.h"
# include "libft.h"

# define WIN_SIZE_PROPORTION 0.9f
# define WIN_TITLE "Cub3D"
// @TODO use enum
# define NORTH 0
# define SOUTH 1
# define EAST 2
# define WEST 3

# define CEIL 4
# define GROUND 5

# define KEY_NUMBER 9
# define KEY_W 0
# define KEY_S 1
# define KEY_A 2
# define KEY_D 3
# define KEY_LEFT 4
# define KEY_RIGHT 5
# define KEY_SHIFT_L 6
# define KEY_CTRL_L 7
# define KEY_CTRL_R 8

# define SPEED 0.02f
# define RUNNING_MODIFIER 2
# define ROTATION_SPEED 0.2f
# define WALL_HEIGHT 800
# define RENDER_DISTANCE 400
// COLLISION_BOX_SIZE always < 1
# define COLLISION_BOX_SIZE 0.2f

# define ZOOM_FOV 10
# define NORMAL_FOV 90
# define SPEED_FOV 110
# define TRANSITION_FOV 1

# define FALSE 0
# define TRUE 1

typedef char	t_bool;

typedef struct s_img {
	void	*addr;
	char	*pixels;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_vec2 {
	float	x;
	float	y;
}	t_vec2;

typedef struct s_player {
	t_vec2	pos;
	t_vec2	speed;
	t_vec2	dir;
	t_vec2	plane;
	float	rotate;
	float	fov;
	char	running;
}	t_player;

typedef struct s_ray {
	t_vec2	pos;
	t_vec2	dir;
	float	delta[2];
}	t_ray;

typedef struct s_nearest_wall {
	float	perceived_distance;
	int		side;
}	t_nearest_wall;

typedef struct s_map {
	t_player	player;
	char		**grid;
	t_img		textures[4];
	int			ceil_color;
	int			ground_color;
	int			width;
	int			height;
	int			has_player;
	char		*texture_initialisation_steps;
	t_bool		ready;
}	t_map;

typedef struct s_inputs_handler {
	char	inputs[KEY_NUMBER];
	t_bool	render_minimap;
	t_bool	active_collisions;
}	t_inputs_handler;

typedef struct	s_minimap
{
//	t_ray			ray;
//	t_nearest_wall	wall;
	int				max_search;
	int				mm_zoom;
}				t_minimap;

typedef struct s_context {
	void				*mlx;
	void				*win;
	t_img				img;
	int					win_width;
	int					win_height;
	t_map				map;
	t_inputs_handler	inputs_handler;
	t_minimap			minimap;	//	To store minimap data. Need to remove magic number!
}	t_context;

// context
int		init_context(t_context *context, char **argv);
void	destroy_context(t_context *context);

// errors
int		basic_error(char *str, int ret);
void	print_error(char *str);

// inputs
int		destroy_hook(t_context *context);
int		keydown_hook(int keycode, t_context *context);
int		keyup_hook(int keycode, t_context *context);
int		motion_hook(int x, int y, t_context *context);
void	compute_inputs(t_context *context);

// images
void	set_img_pixel(t_img *img, int x, int y, int color);
int		get_img_pixel(t_img *img, int x, int y);

// map
int		init_map(t_context *context, char *path);
void	destroy_map(t_map *map, void *mlx);
char	get_map_char(t_map *map, int x, int y);
int		is_wall_map(t_map *map, int x, int y);
void	set_map_char(t_map *map, int x, int y, char c);

// rendering
void	render_main_scene(t_context *context);
void	render_minimap(t_context *context);

// player
void	init_player(t_map *map, char c, t_vec2 pos);
void	rotate_player(t_context *context, float angle);
void	change_player_fov(t_player *player, float fov);
float	get_player_fov(t_player *player);

#endif
