/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/04/19 22:30:16 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"
#include "pipex.h"

void	child_process(int f1, char **cmd1, int end[2], char *const envp[])
{
	if (dup2(f1, STDIN_FILENO) < 0)
		exit(EXIT_FAILURE);
	if (dup2(end[1], STDOUT_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(end[0]);
	close(f1);
	execve(cmd1[0], cmd1, envp);
	exit(EXIT_FAILURE);
}

void	parent_process(t_args args, int f2, char **cmd2, int end[2], char *const envp[])
{
	ssize_t	i;
	int 	status;
	char 	*cmd;
	int ret_access;

	waitpid(-1, &status, 0);
	if (dup2(f2, STDOUT_FILENO) < 0)
		exit(EXIT_FAILURE);
	if (dup2(end[0], STDIN_FILENO) < 0)
		exit(EXIT_FAILURE);
	close(end[1]);
	close(f2);
	ret_access = access(cmd2[0], F_OK | X_OK);
	if (ret_access == 0)
		execve(cmd2[0], cmd2, envp);
	i = -1;
	while (args.paths[++i])
	{
		cmd = ft_strjoin(args.paths[i], cmd2[0]);
		ret_access = access(cmd, F_OK | X_OK);
		if (ret_access == 0)
			execve(cmd, cmd2, envp);
		free(cmd);
	}
	if (ret_access == -1)
		perror(cmd2[0]);
	exit(EXIT_FAILURE);
}

void	pipex(t_args args, char *const envp[])
{
	int		end[2];
	pid_t	parent;

	pipe(end);
	parent = fork();
	if (parent < 0)
		return (perror("Fork"));
	if (!parent)
		child_process(args.infile, args.cmds[0], end, envp);
	else
		parent_process(args, args.outfile, args.cmds[1], end, envp);
}

int	main(int argc, char *const argv[], char *const envp[])
{
	t_args	args;

	if (argc >= 5)
	{
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
