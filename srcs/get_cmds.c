/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 06:17:06 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/01 10:09:48 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	***get_cmds(int argc, char *const argv[])
{
	int		i;
	char	***cmds;

	cmds = (char ***)malloc(sizeof(char **) * (argc - 2));
	if (!cmds)
		return (NULL);
	cmds[argc - 3] = NULL;
	i = 0;
	while (i < argc - 3)
	{
		cmds[i] = ft_split(argv[i + 2], ' ');
		if (!cmds[i])
		{
			while (i--)
				free(cmds[i]);
			free(cmds);
			return (NULL);
		}
		i++;
	}
	return (cmds);
}
