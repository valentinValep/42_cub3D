/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlepille <vlepille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:30:51 by vlepille          #+#    #+#             */
/*   Updated: 2023/10/04 15:06:24 by vlepille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

void	print_error(char *str)
{
	static int	is_already_printed;

	if (!is_already_printed)
	{
		is_already_printed = 1;
		ft_printf_fd(STDERR_FILENO, "Error\n");
	}
	ft_printf_fd(STDERR_FILENO, "%s", str);
}

int	basic_error(char *str, int ret)
{
	print_error(str);
	return (ret);
}
