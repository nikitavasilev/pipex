/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/01 07:52:59 by nvasilev         ###   ########.fr       */
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
#include "libft.h"
#include "pipex.h"

int	intermediate_childs(t_args args, char **cmd, char *const envp[])
{
	ssize_t	i;
	int		ret_access;
	char 	*abs_cmd;
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		perror("Pipe");
	pid = fork();
	if (pid == -1)
		perror("Fork");
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		ret_access = access(cmd[0], F_OK | X_OK);
		if (!ret_access)
			execve(cmd[0], cmd, envp);
		i = -1;
		while (args.paths[++i])
		{
			abs_cmd = ft_strjoin(args.paths[i], cmd[0]);
			ret_access = access(abs_cmd, F_OK | X_OK);
			if (!ret_access)
				return (close(fd[1]), close(fd[0]), execve(abs_cmd, cmd, envp), free(abs_cmd), 0);
			free(abs_cmd);
		}
		if (ret_access)
			perror(cmd[0]);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (EXIT_FAILURE);
}

void	last_child(t_args args, char **last_cmd, char *const envp[])
{
	ssize_t	i;
	char 	*cmd;
	int		ret_access;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("Fork"));
	if (pid == 0)
	{
		ret_access = access(last_cmd[0], F_OK | X_OK);
		if (ret_access == 0)
			execve(last_cmd[0], last_cmd, envp);
		i = -1;
		while (args.paths[++i])
		{
			cmd = ft_strjoin(args.paths[i], last_cmd[0]);
			ret_access = access(cmd, F_OK | X_OK);
			if (ret_access == 0)
				execve(cmd, last_cmd, envp);
		}
		if (ret_access == -1)
			perror(last_cmd[0]);
		exit(EXIT_FAILURE);
	}
}

void	pipex(t_args args, char *const envp[])
{
	unsigned int	i;
	int				status;

	if (dup2(args.infile, STDIN_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(args.infile);
	i = 0;
	while (i < args.cmd_count - 1)
	{
		intermediate_childs(args, args.cmds[i], envp);
		waitpid(-1, &status, 0);
		i++;
	}
	if (dup2(args.outfile, STDOUT_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(args.outfile);
	last_child(args, args.cmds[i], envp);
	waitpid(-1, &status, 0);
}

int	main(int argc, char *const argv[], char *const envp[])
{
	t_args	args;

	if (argc >= 5)
	{
		args.cmd_count = argc - 3;
		args.infile = open(argv[1], O_RDONLY);
		args.outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (args.infile < 0 || args.outfile < 0)
			return (perror("Open"), 1);
		args.paths = get_paths(envp);
		if (!args.paths)
			return (perror("Get_paths"), 1);
		args.cmds = get_cmds(argc, argv);
		if (!args.cmds)
			return (perror("Get_cmds"), 1);
		pipex(args, envp);
	}
	return (0);
}
