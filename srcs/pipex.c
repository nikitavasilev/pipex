/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 05:40:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/16 06:07:32 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "pipex.h"
#include "libft.h"

static int	wait_for_child(pid_t *cpid, unsigned int max_proc)
{
	ssize_t	i;
	int		status;
	int		last_cmd_status;

	i = 0;
	while (i < max_proc)
	{
		waitpid(cpid[i], &status, 0);
		if (cpid[i] == cpid[max_proc - 1])
			last_cmd_status = status;
		i++;
	}
	return (last_cmd_status);
}

static int	*close_write_n_cpy(int fd[3])
{
	close(fd[WRITE_END]);
	if (fd[TEMP_READ_END] > 0)
		close(fd[TEMP_READ_END]);
	fd[TEMP_READ_END] = fd[READ_END];
	return (fd);
}

static int	hd_pl(t_args args, int fd[3], char *const ep[], char *const av[])
{
	ssize_t	i;

	i = -1;
	while (++i < args.cmd_count - 1)
	{
		if (pipe(fd) == -1)
			return (print_err(errno, __FILE__, __LINE__ - 1),
				exit_failure(args, fd, EXIT_FAILURE), 1);
		args.cpid[i] = fork();
		if (args.cpid[i] == -1)
			return (print_err(errno, __FILE__, __LINE__ - 2),
				exit_failure(args, fd, EXIT_FAILURE), 1);
		if (i == 0)
		{
			if (args.cpid[i] == 0)
				here_doc(av[2], fd, args);
			close(fd[WRITE_END]);
			wait(NULL);
		}
		else if (args.cpid[i] == 0 && i != 0)
			inter_children(args, args.cmds[i], ep, fd);
		fd = close_write_n_cpy(fd);
	}
	return (i);
}

static int	pipeline(t_args args, int fd[3], char *const ep[])
{
	ssize_t	i;

	i = -1;
	while (++i < args.cmd_count - 1)
	{
		if (pipe(fd) == -1)
			return (print_err(errno, __FILE__, __LINE__ - 1),
				exit_failure(args, fd, EXIT_FAILURE), 1);
		args.cpid[i] = fork();
		if (args.cpid[i] == -1)
			return (print_err(errno, __FILE__, __LINE__ - 2),
				exit_failure(args, fd, EXIT_FAILURE), 1);
		else if (args.cpid[i] == 0 && i == 0)
			first_child(args, args.cmds[i], ep, fd);
		else if (args.cpid[i] == 0 && i != 0)
			inter_children(args, args.cmds[i], ep, fd);
		fd = close_write_n_cpy(fd);
	}
	return (i);
}

int	pipex(t_args args, char *const envp[], char *const argv[])
{
	ssize_t	i;
	int		status;
	int		fd[3];

	init_tab(fd, 3);
	if (args.heredoc)
		i = hd_pl(args, fd, envp, argv);
	else
		i = pipeline(args, fd, envp);
	args.cpid[i] = fork();
	if (args.cpid[i] == -1)
		return (print_err(errno, __FILE__, __LINE__ - 2),
			exit_failure(args, fd, EXIT_FAILURE), 1);
	if (args.cpid[i] == 0)
		last_child(args, args.cmds[i], envp, fd);
	close(fd[WRITE_END]);
	if (fd[TEMP_READ_END] > 0)
		close(fd[TEMP_READ_END]);
	close(fd[READ_END]);
	close(args.infile);
	close(args.outfile);
	status = wait_for_child(args.cpid, args.cmd_count);
	return (exit_status(status));
}
