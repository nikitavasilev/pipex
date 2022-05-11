/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:29:38 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/09 17:47:15 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "pipex.h"

int	init_args(t_args *args, int argc, char *const argv[], char *const envp[])
{
	args->paths = NULL;
	args->cmds = NULL;
	args->infile = -1;
	args->outfile = -1;
	args->cmd_count = argc - 3;
	args->infile = open(argv[1], O_RDONLY);
	if (args->infile < 0)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (args->outfile < 0)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->paths = get_paths(envp);
	if (!args->paths)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->cmds = get_cmds(argc, argv);
	if (!args->cmds)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	return (1);
}
