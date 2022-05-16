/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:53:56 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/14 06:56:33 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

# define BUFFER_SIZE 32
# define MAX_FD 256

size_t	ft_strlen(const char *s);
int		ft_putchar_fd(int c, int fd);
int		ft_putstr_fd(const char *str, int fd);
char	**ft_split(char const *s, char c);
char	*get_next_line(int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
void	ft_putnbr_fd(int n, int fd);

#endif
