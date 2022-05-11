/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:53:52 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/09 17:56:05 by nvasilev         ###   ########.fr       */
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
void	free_args(t_args args);
void	close_stdio(void);
void	print_err(int errnum, char *str, int line);
void	exit_failure(t_args args, int fd[3]);
int		init_args(t_args *args, int ac, char *const av[], char *const ep[]);
int		is_dir(char *path);
void	exec_cmd(t_args args, char **cmd, char *const envp[]);
void	first_child(t_args args, char **cmd, char *const envp[], int fd[3]);
void	inter_children(t_args args, char **cmd, char *const envp[], int fd[3]);
void	last_child(t_args args, char **cmd, char *const envp[], int fd[3]);

#endif
