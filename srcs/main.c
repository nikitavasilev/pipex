/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/11 23:53:39 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "libft.h"
#include "pipex.h"

int	pipex(t_args args, char *const envp[])
{
	ssize_t	i;
	int		status;
	int		fd[3];
	pid_t	cpid[args.cmd_count];

	i = 0;
	while (i < 3)
		fd[i++] = -1;
	i = -1;
	while (++i < args.cmd_count - 1)
	{
		if (pipe(fd) == -1)
			return (print_err(errno, __FILE__, __LINE__ - 1),
				exit_failure(args, fd), 1);
		cpid[i] = fork();
		if (cpid[i] == -1)
			return (print_err(errno, __FILE__, __LINE__ - 2),
				exit_failure(args, fd), 1);
		if (cpid[i] == 0 && i == 0)
			first_child(args, args.cmds[i], envp, fd);
		else if (cpid[i] == 0 && i != 0)
			inter_children(args, args.cmds[i], envp, fd);
		close(fd[WRITE_END]);
		if (fd[TEMP_READ_END] > 0)
			close(fd[TEMP_READ_END]);
		fd[TEMP_READ_END] = fd[READ_END];
	}
	cpid[i] = fork();
	if (cpid[i] == -1)
		return (print_err(errno, __FILE__, __LINE__ - 2),
			exit_failure(args, fd), 1);
	if (cpid[i] == 0)
		last_child(args, args.cmds[i], envp, fd);
	close(fd[WRITE_END]);
	if (fd[TEMP_READ_END] > 0)
		close(fd[TEMP_READ_END]);
	close(fd[READ_END]);
	close(args.infile);
	close(args.outfile);
	i = 0;
	while (i++ < args.cmd_count)
		wait(&status);
	dprintf(STDERR_FILENO, "errno: %d\n", errno);
	if (errno == -1 || errno == ENOENT)
		return (127);
	if (errno == EACCES)
		return (126);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	main(int argc, char *const argv[], char *const envp[])
{
	t_args	args;
	int		status;

	status = 0;
	if (argc >= 5)
	{
		if (!init_args(&args, argc, argv, envp))
			exit_failure(args, NULL);
		status = pipex(args, envp);
		free_args(args);
	}
//	dprintf(STDERR_FILENO, "status = %d\n", status);
//	dprintf(STDERR_FILENO, "errno = %d\n", errno);
	close_stdio();
	return (status);
}
