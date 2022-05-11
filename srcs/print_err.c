/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:22:40 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/10 10:32:56 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include "libft.h"

void	print_err(int errnum, char *str, int line)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	if (errnum == -1)
		ft_putstr_fd("command not found", STDERR_FILENO);
	else
		ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	if (str)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
	}
	if (line)
	{
		ft_putstr_fd(":", STDERR_FILENO);
		ft_putnbr_fd(line, STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
}
