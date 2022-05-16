/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 03:50:38 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/16 06:07:38 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "libft.h"
#include "pipex.h"

static void	exit_success_hd(t_args args, int fd[3], char *line)
{
	free(line);
	free_args(args);
	close(fd[WRITE_END]);
	close(args.outfile);
	close_stdio();
	exit(EXIT_SUCCESS);
}

static void	print_input(t_args args)
{
	unsigned int	i;
	char			*line;
	char			*tmp;

	line = ft_strdup("");
	i = 0;
	while (i < args.cmd_count - 1)
	{
		tmp = line;
		line = ft_strjoin(tmp, "pipe ");
		free(tmp);
		i++;
	}
	tmp = line;
	line = ft_strjoin(tmp, "heredoc> ");
	free(tmp);
	ft_putstr_fd(line, STDOUT_FILENO);
	free(line);
}

void	here_doc(char *limiter, int fd[3], t_args args)
{
	char	*line;

	line = NULL;
	close(fd[READ_END]);
	print_input(args);
	line = get_next_line(STDIN_FILENO);
	if (!line)
	{
		print_err(errno, __FILE__, __LINE__ - 3);
		exit_failure(args, fd, EXIT_FAILURE);
	}
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			exit_success_hd(args, fd, line);
		write(fd[WRITE_END], line, ft_strlen(line));
		free(line);
		print_input(args);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			print_err(errno, __FILE__, __LINE__ - 3);
			exit_failure(args, fd, EXIT_FAILURE);
		}
	}
}
