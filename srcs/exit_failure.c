/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_failure.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:25:40 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/12 05:58:17 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "pipex.h"

void	exit_failure(t_args args, int fd[3], int status)
{
	if (fd)
	{
		if (fd[READ_END] != -1)
			close(fd[READ_END]);
		if (fd[WRITE_END] != -1)
			close(fd[WRITE_END]);
		if (fd[TEMP_READ_END] != -1)
			close(fd[TEMP_READ_END]);
	}
	if (args.infile != -1)
		close(args.infile);
	if (args.outfile != -1)
		close(args.outfile);
	close_stdio();
	free_args(args);
	exit(status);
}
