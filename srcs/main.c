/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/12 06:02:19 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "pipex.h"

int	main(int argc, char *const argv[], char *const envp[])
{
	t_args	args;
	int		status;

	status = 0;
	if (argc >= 5)
	{
		if (!init_args(&args, argc, argv, envp))
			exit_failure(args, NULL, EXIT_FAILURE);
		status = pipex(args, envp);
		free_args(args);
	}
	else
	{
		ft_putstr_fd(USAGE, STDERR_FILENO);
		status = 1;
	}
	close_stdio();
	return (status);
}
