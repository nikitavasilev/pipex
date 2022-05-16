/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/16 04:58:47 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft.h"
#include "pipex.h"

int	main(int argc, char *const argv[], char *const envp[])
{
	t_args	args;
	int		status;

	status = 0;
	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")) == 0)
		{
			if (argc < 6)
				return (ft_putstr_fd(US_HD, STDERR_FILENO), EXIT_FAILURE);
			init_args_hd(&args, argc, argv, envp);
		}
		else if (!init_args(&args, argc, argv, envp))
			exit_failure(args, NULL, EXIT_FAILURE);
		status = pipex(args, envp, argv);
		free_args(args);
	}
	else
	{
		ft_putstr_fd(USAGE, STDERR_FILENO);
		status = EXIT_FAILURE;
	}
	close_stdio();
	return (status);
}
