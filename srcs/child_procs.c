/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_procs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:48:21 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/12 05:55:43 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "pipex.h"

void	first_child(t_args args, char **cmd, char *const envp[], int fd[3])
{
	if (args.infile != -1)
	{
		if (dup2(args.infile, STDIN_FILENO) < 0)
			return (print_err(errno, __FILE__, __LINE__ - 1),
				exit_failure(args, fd, EXIT_FAILURE));
	}
	if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd, EXIT_FAILURE));
	close(fd[READ_END]);
	close(args.infile);
	exec_cmd(args, cmd, envp);
	return (exit_failure(args, fd, EXIT_FAILURE));
}

void	inter_children(t_args args, char **cmd, char *const envp[], int fd[3])
{
	if (dup2(fd[TEMP_READ_END], STDIN_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd, EXIT_FAILURE));
	if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd, EXIT_FAILURE));
	close(fd[READ_END]);
	exec_cmd(args, cmd, envp);
	return (exit_failure(args, fd, EXIT_FAILURE));
}

void	last_child(t_args args, char **cmd, char *const envp[], int fd[3])
{
	if (dup2(fd[TEMP_READ_END], STDIN_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd, EXIT_FAILURE));
	if (dup2(args.outfile, STDOUT_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd, EXIT_FAILURE));
	close(args.outfile);
	close(fd[READ_END]);
	close(fd[WRITE_END]);
	exec_cmd(args, cmd, envp);
	if (errno == -1 || errno == ENOENT)
		return (exit_failure(args, fd, 127));
	if (errno == EACCES || errno == EISDIR
		|| errno == EPERM || errno == ENOEXEC || errno == EINVAL)
		return (exit_failure(args, fd, 126));
	return (exit_failure(args, fd, EXIT_FAILURE));
}
