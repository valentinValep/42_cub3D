#include "cub3d.h"

static t_vec2	get_dir_from_char(char c)
{
	if (c == 'N')
		return ((t_vec2){0, -1});
	else if (c == 'S')
		return ((t_vec2){0, 1});
	else if (c == 'E')
		return ((t_vec2){1, 0});
	return ((t_vec2){-1, 0});
}

void	init_player(t_map *map, char c, t_vec2 pos)
{
	map->player.pos = (t_vec2){pos.x + 0.5, pos.y + 0.5};
	map->player.speed = (t_vec2){0., 0.};
	map->player.rotate = 0.;
	map->player.dir = get_dir_from_char(c);
	map->player.plane = (t_vec2){-map->player.dir.y * 2, map->player.dir.x * 2};
	map->has_player = 1;
}

void	rotate_player(t_context *context, float angle)
{
	const float	old_dir_x = context->map.player.dir.x;
	const float	old_plane_x = context->map.player.plane.x;

	context->map.player.dir.x = context->map.player.dir.x * cos(angle)
		- context->map.player.dir.y * sin(angle);
	context->map.player.dir.y = old_dir_x * sin(angle)
		+ context->map.player.dir.y * cos(angle);
	context->map.player.plane.x = context->map.player.plane.x * cos(angle)
		- context->map.player.plane.y * sin(angle);
	context->map.player.plane.y = old_plane_x * sin(angle)
		+ context->map.player.plane.y * cos(angle);
}

void	change_player_fov(t_player *player, float fov)
{
	player->fov = fov;
}
