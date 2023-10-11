/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing_texture.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:53:25 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:53:32 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdlib.h>

int	init_texture(t_context *context, int face, char *path)
{
	int	i;

	i = 0;
	while (path[i] && !is_whitespace(path[i]))
		i++;
	if (is_whitespace(path[i]))
	{
		path[i++] = 0;
		while (path[i] && is_whitespace_no_newline(path[i]))
			i++;
		if (path[i] && path[i] != '\n')
			return (print_error(
					"Invalid character after texture path\n"), EXIT_FAILURE);
	}
	context->map.textures[face].addr = mlx_xpm_file_to_image(
			context->mlx, path, &context->map.textures[face].width,
			&context->map.textures[face].height);
	if (!context->map.textures[face].addr)
		return (print_error("Failed to load texture\n"), EXIT_FAILURE);
	context->map.textures[face].pixels = mlx_get_data_addr(
			context->map.textures[face].addr,
			&context->map.textures[face].bpp,
			&context->map.textures[face].line_len,
			&context->map.textures[face].endian);
	return (0);
}
