/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:14:57 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/14 02:21:25 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

void	free_err_join(char *path, char *tmp, char **paths)
{
	size_t	i;

	free(tmp);
	free(path);
	i = 0;
	while (paths[i])
		free(paths[i++]);
}

void	free_tab(char **tab)
{
	unsigned int	i;

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

static void	free_cmds(char ***cmds)
{
	unsigned int	i;

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
	free(args.cpid);
}
