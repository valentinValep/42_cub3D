#include "cub3d.h"

void	set_img_pixel(t_img *img, int x, int y, int color)
{
	(((int *)img->pixels))[x + y * WIN_WIDTH] = color;
}

int	get_img_pixel(t_img *img, int x, int y)
{
	return (((int *)img->pixels)[x + y * WIN_WIDTH]);
}
