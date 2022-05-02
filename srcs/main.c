/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/01 12:40:35 by nvasilev         ###   ########.fr       */
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

void	close_stds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void	init_args(t_args *args)
{
	args->paths = NULL;
	args->cmds = NULL;
	args->cmd_count = 0;
}

void	free_tab(char **tab)
{
	int i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_cmds(char ***cmds)
{
	int i;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
	{
		free_tab(cmds[i]);
		i++;
	}
	free(cmds);
}

void	free_args(t_args args)
{
	free_cmds(args.cmds);
	free_tab(args.paths);
}

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
		//close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (!cmd[0])
		{
			free_args(args);
			close(fd[0]);
			close(fd[1]);
			close(args.outfile);
			close_stds();
			exit(EXIT_FAILURE);
		}
		ret_access = access(cmd[0], F_OK | X_OK);
		if (!ret_access)
			return (close(fd[0]), close(fd[1]), execve(cmd[0], cmd, envp));
		i = -1;
		while (args.paths[++i])
		{
			abs_cmd = ft_strjoin(args.paths[i], cmd[0]);
			if (!abs_cmd)
				perror("Malloc");
			ret_access = access(abs_cmd, F_OK | X_OK);
			if (!ret_access)
				return (close(fd[0]), close(fd[1]), execve(abs_cmd, cmd, envp));
			free(abs_cmd);
		}
		if (ret_access)
		{
			//perror(NULL);
			char *strs[] = {"/bin/cat", "Makefile", NULL};
			//dprintf(STDERR_FILENO, "%s\n", strerror(errno));
			if (execve("/bin/cat", strs, envp) == -1)
				perror("Execve");
			dprintf(STDERR_FILENO, "Command not found: %s\n", cmd[0]);
			free_args(args);
			close(fd[0]);
			close(fd[1]);
			close(args.outfile);
			close_stds();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
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
			if (!cmd)
				perror("Malloc");
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
	if (args.cmds[i][0])
		last_child(args, args.cmds[i], envp);
	waitpid(-1, &status, 0);
}

int	main(int argc, char *const argv[], char *const envp[])
{
	t_args	args;

	if (argc >= 5)
	{
		init_args(&args);
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
		free_args(args);
	}
	close_stds();
	return (0);
}
