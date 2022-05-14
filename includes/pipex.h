/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:53:52 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/14 02:19:07 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stddef.h>

# define READ_END 0
# define WRITE_END 1
# define TEMP_READ_END 2
# define USAGE "Usage: ./pipex [infile] [command] [command] ... [outfile]\n"

typedef struct s_args
{
	char			**paths;
	char			***cmds;
	int				infile;
	int				outfile;
	unsigned int	cmd_count;
	pid_t			*cpid;
}	t_args;

char	**get_paths(char *const envp[]);
char	***get_cmds(int argc, char *const argv[]);
void	free_args(t_args args);
void	free_tab(char **tab);
void	free_err_join(char *path, char *tmp, char **paths);
void	close_stdio(void);
void	print_err(int errnum, char *str, int line);
void	print_err_open(int errnum, char *path);
void	exit_failure(t_args args, int fd[3], int status);
int		init_args(t_args *args, int ac, char *const av[], char *const ep[]);
int		is_dir(char *path);
void	exec_cmd(t_args args, char **cmd, char *const envp[]);
void	first_child(t_args args, char **cmd, char *const envp[], int fd[3]);
void	inter_children(t_args args, char **cmd, char *const envp[], int fd[3]);
void	last_child(t_args args, char **cmd, char *const envp[], int fd[3]);
int		pipex(t_args args, char *const envp[]);

#endif
