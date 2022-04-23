/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/04/23 06:07:41 by nvasilev         ###   ########.fr       */
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

void	first_child(t_args args, int infile, char **cmd1, int end[2], char *const envp[])
{
	ssize_t	i;
	int		ret_access;
	char 	*abs_cmd;

	if (dup2(infile, STDIN_FILENO) < 0)
		exit(EXIT_FAILURE);
	if (dup2(end[1], STDOUT_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(end[0]);
	close(infile);
	ret_access = access(cmd1[0], F_OK | X_OK);
	if (!ret_access)
		execve(cmd1[0], cmd1, envp);
	i = -1;
	while (args.paths[++i])
	{
		abs_cmd = ft_strjoin(args.paths[i], cmd1[0]);
		ret_access = access(abs_cmd, F_OK | X_OK);
		if (!ret_access)
			execve(abs_cmd, cmd1, envp);
		free(abs_cmd);
	}
	if (ret_access)
		perror(cmd1[0]);
	exit(EXIT_FAILURE);
}

void	child_process(t_args args, int infile, char **cmd1, int end[2], char *const envp[])
{
	ssize_t	i;
	int		ret_access;
	char 	*abs_cmd;

	if (dup2(infile, STDIN_FILENO) < 0)
		exit(EXIT_FAILURE);
	if (dup2(end[1], STDOUT_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(end[0]);
	close(infile);
	ret_access = access(cmd1[0], F_OK | X_OK);
	if (!ret_access)
		execve(cmd1[0], cmd1, envp);
	i = -1;
	while (args.paths[++i])
	{
		abs_cmd = ft_strjoin(args.paths[i], cmd1[0]);
		ret_access = access(abs_cmd, F_OK | X_OK);
		if (!ret_access)
			execve(abs_cmd, cmd1, envp);
		free(abs_cmd);
	}
	if (ret_access)
		perror(cmd1[0]);
	exit(EXIT_FAILURE);
}

void	parent_process(t_args args, int outfile, char **last_cmd, int end[2], char *const envp[])
{
	ssize_t	i;
	int 	status;
	char 	*cmd;
	int		ret_access;

	waitpid(-1, &status, 0);
	if (dup2(outfile, STDOUT_FILENO) < 0)
		exit(EXIT_FAILURE);
	if (dup2(end[0], STDIN_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(end[1]);
	close(outfile);
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
		free(cmd);
	}
	if (ret_access == -1)
		perror(last_cmd[0]);
	exit(EXIT_FAILURE);
}

void	pipex(t_args args, char *const envp[])
{
	unsigned int	i;
	int				end[2];
	pid_t			parent;

	i = 0;
	first_child(args, args.infile, args.cmds[i], end, envp);
	i++;
	while (i < args.cmd_count - 1)
	{
		wait(NULL);
		if (pipe(end) < 0)
			return (perror("Pipe"), exit(EXIT_FAILURE));
		parent = fork();
		if (parent < 0)
			return (perror("Fork"));
		if (!parent)
			child_process(args, args.infile, args.cmds[i], end, envp);
		i++;
	}
	parent_process(args, args.outfile, args.cmds[i], end, envp);
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
		/*while (args.paths[0])
		{
			printf("%s\n", args.paths[0]);
			args.paths++;
		}
		int i = 0;
		while (args.cmds[i])
		{
			while (args.cmds[i][0])
			{
				printf("%s\n", args.cmds[i][0]);
				args.cmds[i]++;
			}
			i++;
		}*/
	}
	return (0);
}
