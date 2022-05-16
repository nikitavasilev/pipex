/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:29:38 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/16 06:01:01 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "pipex.h"

void	init_tab(int *tab, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (i < size)
		tab[i++] = 0;
}

int	init_args_hd(t_args *args, int argc, char *const argv[], char *const envp[])
{
	args->paths = NULL;
	args->cmds = NULL;
	args->infile = -1;
	args->outfile = -1;
	args->cmd_count = argc - 4;
	args->heredoc = 1;
	args->cpid = NULL;
	args->outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (args->outfile < 0)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->paths = get_paths(envp);
	if (!args->paths)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->cmds = get_cmds(args, argc, argv);
	if (!args->cmds)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->cpid = (pid_t *)malloc(sizeof(pid_t) * args->cmd_count);
	if (!args->cpid)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	return (1);
}

int	init_args(t_args *args, int argc, char *const argv[], char *const envp[])
{
	args->paths = NULL;
	args->cmds = NULL;
	args->infile = -1;
	args->outfile = -1;
	args->cmd_count = argc - 3;
	args->cpid = NULL;
	args->infile = open(argv[1], O_RDONLY);
	args->heredoc = 0;
	if (args->infile < 0)
		print_err_open(errno, argv[1]);
	args->outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (args->outfile < 0)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->paths = get_paths(envp);
	if (!args->paths)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->cmds = get_cmds(args, argc, argv);
	if (!args->cmds)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	args->cpid = (pid_t *)malloc(sizeof(pid_t) * args->cmd_count);
	if (!args->cpid)
		return (print_err(errno, __FILE__, __LINE__ - 2), 0);
	return (1);
}
