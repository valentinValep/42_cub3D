#include "cub3d.h"

void	set_img_pixel(t_img *img, int x, int y, int color)
{
	if (x >= 0 && x < img->width && y >= 0 && y < img->height)
		(((int *)img->pixels))[x + y * img->width] = color;
}

int	get_img_pixel(t_img *img, int x, int y)
{
	return (((int *)img->pixels)[x + y * img->width]);
}
