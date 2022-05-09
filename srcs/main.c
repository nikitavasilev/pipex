/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:54:03 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/09 07:50:52 by nvasilev         ###   ########.fr       */
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

void	error_exit(int errnum, char *str)
{
	ft_putstr_fd("pipex: ", 2);
	if (errnum == -1)
		ft_putstr_fd("command not found", 2);
	else
		ft_putstr_fd(strerror(errnum), 2);
	ft_putstr_fd(": ", 2);
	if (str)
		ft_putstr_fd(str, 2);
	ft_putchar_fd('\n', 2);
}

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

void	exit_failure(t_args args, int fd[2])
{
	free_args(args);
	if (fd)
	{
		close(fd[0]);
		close(fd[1]);
	}
	close(args.outfile);
	close_stds();
	exit(EXIT_FAILURE);
}

int	is_dir(char *path)
{
	int	fd;

	fd = open(path, O_RDWR);
	if ((fd == -1) && (errno == EISDIR))
	{
		errno = EACCES;
		return (EISDIR);
	}
	if (fd > 0)
		close(fd);
	return (0);
}

void	first_child(t_args args, char **cmd, char *const envp[], int fd[2])
{
	ssize_t	i;
	int		ret_access;
	char 	*abs_cmd;

	if (dup2(args.infile, STDIN_FILENO ) < 0)
		return (perror("dup2 infile"), exit(EXIT_FAILURE));
	if (dup2(fd[WRITE_END], STDOUT_FILENO ) == -1)
		return (perror("dup2 pipefd"),exit(EXIT_FAILURE));
	close(fd[READ_END]);
	close(fd[WRITE_END]);
	close(args.infile);
	if (!cmd[0])
	{
		error_exit(EACCES, cmd[0]);
		exit_failure(args, fd);
	}
	ret_access = access(cmd[0], F_OK | X_OK);
	if (!ret_access && !is_dir(cmd[0]))
		execve(cmd[0], cmd, envp);
	i = -1;
	while (args.paths[++i])
	{
		abs_cmd = ft_strjoin(args.paths[i], cmd[0]);
		if (!abs_cmd)
			perror("Malloc");
		ret_access = access(abs_cmd, F_OK | X_OK);
		if (!ret_access && !is_dir(abs_cmd))
			execve(abs_cmd, cmd, envp);
		free(abs_cmd);
	}
	if (cmd[0][0] != '/')
		errno = -1;
	error_exit(errno, cmd[0]);
	exit_failure(args, fd);
}

void	intermediate_children(t_args args, char **cmd, char *const envp[], int fd[3])
{
	ssize_t	i;
	int		ret_access;
	char 	*abs_cmd;

	if (dup2(fd[2], STDIN_FILENO) < 0)
		return (perror("dup2 infile"), exit(EXIT_FAILURE));
	if (dup2(fd[WRITE_END], STDOUT_FILENO ) == -1)
		return (perror("dup2 pipefd"),exit(EXIT_FAILURE));
	close(fd[2]);
	close(fd[WRITE_END]);
	close(fd[READ_END]);
	dprintf(STDERR_FILENO, "%s\n", cmd[0]);
	if (!cmd[0])
	{
		error_exit(EACCES, cmd[0]);
		exit_failure(args, fd);
	}
	ret_access = access(cmd[0], F_OK | X_OK);
	if (!ret_access && !is_dir(cmd[0]))
		execve(cmd[0], cmd, envp);
	i = -1;
	while (args.paths[++i])
	{
		abs_cmd = ft_strjoin(args.paths[i], cmd[0]);
		if (!abs_cmd)
			perror("Malloc");
		ret_access = access(abs_cmd, F_OK | X_OK);
		if (!ret_access && !is_dir(abs_cmd))
			execve(abs_cmd, cmd, envp);
		free(abs_cmd);
	}
	if (cmd[0][0] != '/')
		errno = -1;
	error_exit(errno, cmd[0]);
	exit_failure(args, fd);
}

void	last_child(t_args args, char **last_cmd, char *const envp[], int fd[3])
{
	ssize_t	i;
	char 	*cmd;
	int		ret_access;

	if (dup2(fd[READ_END], STDIN_FILENO) < 0)
	  	return (perror("dup2 infile"), exit(EXIT_FAILURE));
	if (dup2(args.outfile, STDOUT_FILENO) < 0)
		return (perror("dup2 outfile"), exit(EXIT_FAILURE));
	close(args.outfile);
	close(fd[READ_END]);
	close(fd[WRITE_END]);
	close(fd[2]);
	ret_access = access(last_cmd[0], F_OK | X_OK);
	if (!ret_access && !is_dir(last_cmd[0]))
		execve(last_cmd[0], last_cmd, envp);
	i = -1;
	while (args.paths[++i])
	{
		cmd = ft_strjoin(args.paths[i], last_cmd[0]);
		if (!cmd)
			perror("Malloc");
		ret_access = access(cmd, F_OK | X_OK);
		if (!ret_access && !is_dir(cmd))
			execve(cmd, last_cmd, envp);
		free(cmd);
	}
	if (last_cmd[0][0] != '/')
		errno = -1;
	error_exit(errno, last_cmd[0]);
	exit_failure(args, NULL);
}

void	pipex(t_args args, char *const envp[])
{
	ssize_t	i;
	int		status;
	//int		fd[2];
	int		fd[3];
	pid_t	cpid;

	i = -1;
	while (++i < args.cmd_count - 1)
	{
		if (pipe(fd) == -1)
			perror("Pipe");
		cpid = fork();
		if (cpid == 0 && i == 0)
			first_child(args, args.cmds[i], envp, fd);
		else if (cpid == 0 && i != 0)
			intermediate_children(args, args.cmds[i], envp, fd);
		close(fd[WRITE_END]);
		close(fd[2]);
		fd[2] = fd[0];
	}
	if (args.cmds[i][0])
	{
		cpid = fork();
		if (cpid == 0)
			last_child(args, args.cmds[i], envp, fd);
	}
	close(fd[READ_END]);
	close(fd[WRITE_END]);
	close(fd[2]);
	close(args.infile);
	close(args.outfile);
	i = 0;
	while (i++ < args.cmd_count)
		wait(&status);
	//while (wait(&status) > 0);
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
