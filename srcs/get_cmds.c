/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 06:17:06 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/16 04:59:24 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"
#include "libft.h"

char	***get_cmds(t_args *args, int argc, char *const argv[])
{
	int		i;
	char	***cmds;

	cmds = (char ***)malloc(sizeof(char **) * (argc - 2 - args->heredoc));
	if (!cmds)
		return (NULL);
	cmds[argc - (3 + args->heredoc)] = NULL;
	i = args->heredoc;
	while (i < argc - 3)
	{
		cmds[i - args->heredoc] = ft_split(argv[i + 2], ' ');
		if (!cmds[i - args->heredoc])
		{
			while (i--)
				free(cmds[i + args->heredoc]);
			free(cmds);
			return (NULL);
		}
		i++;
	}
	return (cmds);
}
