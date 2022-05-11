/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_procs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:48:21 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/10 09:32:35 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "pipex.h"

void	first_child(t_args args, char **cmd, char *const envp[], int fd[3])
{
	if (dup2(args.infile, STDIN_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd));
	if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd));
	close(fd[READ_END]);
	//close(fd[WRITE_END]);
	close(args.infile);
	exec_cmd(args, cmd, envp);
	return (exit_failure(args, fd));
}

void	inter_children(t_args args, char **cmd, char *const envp[], int fd[3])
{
	if (dup2(fd[TEMP_READ_END], STDIN_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd));
	if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd));
	close(fd[READ_END]);
//	close(fd[TEMP_READ_END]);
//	close(fd[WRITE_END]);
//	close(args.infile);
	exec_cmd(args, cmd, envp);
	return (exit_failure(args, fd));
}

void	last_child(t_args args, char **cmd, char *const envp[], int fd[3])
{
	if (dup2(fd[TEMP_READ_END], STDIN_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd));
	if (dup2(args.outfile, STDOUT_FILENO) < 0)
		return (print_err(errno, __FILE__, __LINE__ - 1),
			exit_failure(args, fd));
	close(args.outfile);
	close(fd[READ_END]);
	close(fd[WRITE_END]);
//	close(args.infile);
	exec_cmd(args, cmd, envp);
	dprintf(STDERR_FILENO, "errno: %d\n", errno);
	if (errno == -1 || errno == ENOENT)
	{
		dprintf(STDERR_FILENO, "127\n");
		//signal(SIGPIPE, SIG_IGN);
		exit(127);
	}
	if (errno == EACCES || errno == EISDIR || errno == EPERM || errno == ENOEXEC)
		exit(126);
	return (exit_failure(args, fd));
}
