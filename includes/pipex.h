/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:53:52 by nvasilev          #+#    #+#             */
/*   Updated: 2022/04/17 08:35:33 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_args
{
	char	**paths;
	char	***cmds;
	int		infile;
	int		outfile;
}	t_args;

char	**get_paths(char *const envp[]);
char	***get_cmds(int argc, char *const argv[]);

#endif
