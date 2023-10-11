/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguarrac <fguarrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:53:50 by fguarrac          #+#    #+#             */
/*   Updated: 2023/10/11 15:53:51 by fguarrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	get_first_char(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	return (line[i]);
}

int	pass_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_whitespace_no_newline(line[i]))
		i++;
	return (i);
}
