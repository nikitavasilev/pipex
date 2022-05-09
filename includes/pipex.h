/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:53:52 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/09 07:51:35 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define READ_END 0
# define WRITE_END 1
# define TEMP_READ_END 2

typedef struct s_args
{
	char			**paths;
	char			***cmds;
	int				infile;
	int				outfile;
	unsigned int	cmd_count;
}	t_args;

char	**get_paths(char *const envp[]);
char	***get_cmds(int argc, char *const argv[]);

#endif
