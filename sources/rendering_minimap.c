#include "cub3d.h"
#include <stdlib.h>

#define MAP_SCALE 3


#define MAX_FLOOD 5
enum flood_dir
{
	E_NORD,
	E_EAST,
	E_SOUTH,
	E_WEST
};

struct s_minimap
{
	char	**grid;
	t_vec2	start;
	t_vec2	center;
	int		flood_counter[4];
};


#include <stdio.h>

void	render_player(t_context *context)
{
	const int	x = context->map.player.pos.x * MAP_SCALE
		+ context->win_width - context->map.width * MAP_SCALE;
	const int	y = context->map.player.pos.y * MAP_SCALE;

	set_img_pixel(&context->img, x, y, 0x0000FF);
	set_img_pixel(&context->img, x - 1, y - 1, 0x0000FF);
	set_img_pixel(&context->img, x - 1, y, 0x0000FF);
	set_img_pixel(&context->img, x - 1, y + 1, 0x0000FF);
	set_img_pixel(&context->img, x, y - 1, 0x0000FF);
	set_img_pixel(&context->img, x, y + 1, 0x0000FF);
	set_img_pixel(&context->img, x + 1, y - 1, 0x0000FF);
	set_img_pixel(&context->img, x + 1, y, 0x0000FF);
	set_img_pixel(&context->img, x + 1, y + 1, 0x0000FF);
}

static void	draw_square(t_context *context, int x, int y)
{
	for (int i = 0; i < 5/*running ? 3 : 5*/; i++)
		for (int j = 0; j < 5; j++)
			set_img_pixel(&context->img, x + j, y + i, 0xFFFFFF);
}

static void	search_walls(t_context *context, struct s_minimap minimap)
{
	if (minimap.grid[(int)minimap.start.y][(int)minimap.start.x] == '1')
	{
//		dprintf(1, "Found wall\n");
//		dprintf(1, "minimap.x : %d\tminimap.y: %d\n", (int)minimap_center.x, (int)minimap_center.y);
//		dprintf(1, "start.x : %d\tstart.y: %d\n", (int)start.x, (int)start.y);
		draw_square(context, ((int)minimap.center.x) + (((int)minimap.start.x - (int)context->map.player.pos.x) * 5), ((int)minimap.center.y) + (((int)minimap.start.y - (int)context->map.player.pos.y) * 5));	//	Magic number
		return ;
	}
	if ((int)minimap.start.x - 1 >= 0 && minimap.flood_counter[E_WEST] < MAX_FLOOD && minimap.grid[(int)minimap.start.y][(int)minimap.start.x - 1] != 'F')
	{
		if (minimap.grid[(int)minimap.start.y][(int)minimap.start.x - 1] != '1')
			minimap.grid[(int)minimap.start.y][(int)minimap.start.x - 1] = 'F';
		minimap.flood_counter[E_WEST]++;
		minimap.start.x--;
		search_walls(context, minimap);
	}
	if ((int)minimap.start.x + 1 < context->map.width && minimap.flood_counter[E_EAST] < MAX_FLOOD && minimap.grid[(int)minimap.start.y][(int)minimap.start.x + 1] != 'F')
	{
		if (minimap.grid[(int)minimap.start.y][(int)minimap.start.x + 1] != '1')
			minimap.grid[(int)minimap.start.y][(int)minimap.start.x + 1] = 'F';
		minimap.flood_counter[E_EAST]++;
		minimap.start.x++;
		search_walls(context, minimap);
	}
	if ((int)minimap.start.y - 1 >= 0 && minimap.flood_counter[E_NORD] < MAX_FLOOD && minimap.grid[(int)minimap.start.y - 1][(int)minimap.start.x] != 'F')
	{
		if (minimap.grid[(int)minimap.start.y - 1][(int)minimap.start.x] != '1')
			minimap.grid[(int)minimap.start.y - 1][(int)minimap.start.x] = 'F';
		minimap.flood_counter[E_NORD]++;
		minimap.start.y--;
		search_walls(context, minimap);
	}
	if ((int)minimap.start.y + 1 < context->map.height && minimap.flood_counter[E_SOUTH] < MAX_FLOOD && minimap.grid[(int)minimap.start.y + 1][(int)minimap.start.x] != 'F')
	{
		if (minimap.grid[(int)minimap.start.y + 1][(int)minimap.start.x] != '1')
			minimap.grid[(int)minimap.start.y + 1][(int)minimap.start.x] = 'F';
		minimap.flood_counter[E_SOUTH]++;
		minimap.start.y++;
		search_walls(context, minimap);
	}
}

void	render_minimap(t_context *context)
{
//	char	c;
//	(void)context;
//
//	for (int y = 0; y < context->map.height; y++)
//		for (int x = 0; x < context->map.width; x++)
//		{
//			for (int k = 0; k < MAP_SCALE; k++)
//				for (int l = 0; l < MAP_SCALE; l++)
//				{
//					c = is_wall_map(&context->map, x, y);
//					set_img_pixel(
//						&context->img,
//						x * MAP_SCALE + k + context->win_width - context->map.width * MAP_SCALE,
//						y * MAP_SCALE + l,
//						c ? 0xFF0000 : 0xFFFFFF);
//				}
//		}
//	render_player(context);

	//	calc minimap center position on screen.	//	Save in variable!
	set_img_pixel(&context->img, (7 * (context->win_width / 8))/* - MARGIN*/, (5 * (context->win_height / 6))/* - MARGIN*/, 0XFFFFFF);	//	Test pixel
	//	render rays
		//for (int i = 0; i < 3; i++)
			//	draw minimap ray (with length depending on character speed / running)	//	need to import and adapt my line drawing function from fdf.
	//	render walls
		//	draw surrounding walls (with radius depending on character speed / running)
			//	draw squares (with size depending on character speed / running), (with color depending on orientation?)	//	make function
//draw_square(context, (7 * (context->win_width / 8)) - 20, (5 * (context->win_height / 6)) - 20);	//
//draw_square(context, (7 * (context->win_width / 8)) - 20, (5 * (context->win_height / 6)) - 15);	// test squares
//draw_square(context, (7 * (context->win_width / 8)) - 15, (5 * (context->win_height / 6)) - 20);	//


	//	Player position on map.
		//context->map.grid[(int)(context->map.player.pos.x)][(int)(context->map.player.pos.y)]	//	use var for player position to shorten code.
	//	Search surrounding walls for a max of n squares.	//	Set n depending on running or not
		char				**tmp_grid;					//	Need to find a better way!				//	Alloc once and for all when parsing initial map.
		struct s_minimap	minimap;

		if (!(tmp_grid = (char **)malloc(sizeof(char *) * (context->map.height))))
			return ;
		for (int i = 0; i < context->map.height; i++)
			if (!(tmp_grid[i] = (char *)malloc(sizeof(char) * (context->map.width))))
			{
				for (--i; i >= 0; i--)
					free(tmp_grid[i]);
				return ;
			}

		for (int i = 0; i < context->map.height; i++)
			for (int j = 0; j < context->map.width; j++)
				tmp_grid[i][j] = context->map.grid[i][j];		//	reset here every time

		minimap.grid = tmp_grid;
		minimap.start = (t_vec2){(context->map.player.pos.x), (context->map.player.pos.y)};
		minimap.center = (t_vec2){(7 * (context->win_width / 8)), (5 * (context->win_height / 6))};
		minimap.flood_counter[0] = 0;
		minimap.flood_counter[1] = 0;
		minimap.flood_counter[2] = 0;
		minimap.flood_counter[3] = 0;

		search_walls(context, minimap);
		for (int i = 0; i < context->map.height; i++)
			free(minimap.grid[i]);
		free(minimap.grid);
}
