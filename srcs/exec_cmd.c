/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:42:17 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/12 05:57:26 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "libft.h"
#include "pipex.h"

static int	is_null(char *str)
{
	if (!str)
	{
		errno = EINVAL;
		return (errno);
	}
	return (0);
}

void	exec_cmd(t_args args, char **cmd, char *const envp[])
{
	ssize_t	i;
	char	*abs_cmd;
	int		ret_access;

	if (is_null(cmd[0]))
		return (print_err(errno, cmd[0], 0));
	i = -1;
	while (args.paths[++i] && cmd[0])
	{
		abs_cmd = ft_strjoin(args.paths[i], cmd[0]);
		if (!abs_cmd)
			return (print_err(errno, __FILE__, __LINE__ - 2));
		ret_access = access(abs_cmd, F_OK | X_OK);
		if (!ret_access && !is_dir(abs_cmd))
			execve(abs_cmd, cmd, envp);
		free(abs_cmd);
	}
	if (!ft_strchr(cmd[0], '/'))
		errno = -1;
	ret_access = access(cmd[0], F_OK | X_OK);
	if (!ret_access && !is_dir(cmd[0]))
		execve(cmd[0], cmd, envp);
	print_err(errno, cmd[0], 0);
}
