/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/04/17 04:51:24 by nvasilev         ###   ########.fr       */
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

int	main(int argc, char *const argv[], char *const envp[])
{
	char	**paths;

	(void)argc;
	(void)argv;
	(void)envp;
	paths = get_paths(envp);
	while (paths[0])
	{
		printf("%s\n", paths[0]);
		paths++;
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
