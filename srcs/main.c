/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/04/17 08:38:14 by nvasilev         ###   ########.fr       */
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

/*void	pipex(int infile, int outfile, char *const argv[], char *const envp[])
{
	int   end[2];
	pid_t parent;

	pipe(end);
	parent = fork();
	if (parent < 0)
		return (perror("Fork: "));
	if (!parent) // if fork() returns 0, we are in the child process
		child_process(infile, argv[1]);
	else
		parent_process(outfile, argv[4]);
}*/

void	pipex(int infile, int outfile, char *const argv[], char *const envp[])
{
	int		end[2];
	pid_t	parent;

	pipe(end);
	parent = fork();
	if (parent < 0)
		return (perror("Fork:"));
	if (!parent)
		child_process(infile, argv[1]);
	else
		parent_process(outfile, argv[argc - 1]);
}

int	main(int argc, char *const argv[], char *const envp[])
{
	char	**paths;
	char	***cmds;
	int		infile;
	int		outfile;
	t_args	args;

	args = (t_args) {
		.paths = get_paths(envp),
		.cmds = get_cmds(argc, argv),
		.infile = 0,
		.outfile = 1
	};

	if (argc >= 5)
	{
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (infile < 0 || outfile < 0)
			return (perror("Open:"), 1);
		paths = get_paths(envp);
		if (!paths)
			return (perror("get_paths:"), 1);
		cmds = get_cmds(argc, argv);
		if (!cmds)
			return (perror("get_cmds:"), 1);
		pipex(infile, outfile, envp, paths, cmds);
		/*while (paths[0])
		{
			printf("%s\n", paths[0]);
			paths++;
		}
		i = 0;
		while (cmds[i])
		{
			while (cmds[i][0])
			{
				printf("%s\n", cmds[i][0]);
				cmds[i]++;
			}
			i++;
		}*/
	}
	return (0);
}

/*
int main(int ac, char **ag, char **envp)
{
	int	infile;
	int	outfile;

	infile = open(ag[1], O_RDONLY);
	outfile = open(ag[4], O_CREAT | O_RDWR | O_TRUNC, 0644); // "-rw-r--r--"
	if (infile < 0 || outfile < 0)
		return (-1);
	pipex(infile, outfile, ag, envp);
	return (0);
}*/
