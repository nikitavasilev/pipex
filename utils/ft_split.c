/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:50:34 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/12 06:51:47 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	str_fill(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static size_t	count_words(const char *str, char c)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str != c)
		{
			count++;
			while (*str != c && *str)
				str++;
		}
		else
			str++;
	}
	return (count);
}

static char	**free_if_error(char **strs)
{
	size_t	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	row;
	size_t	start;
	char	**res;

	res = malloc((count_words(s, c) + 1) * sizeof(res));
	if (!s || !res)
		return (NULL);
	row = 0;
	i = 0;
	while (row < count_words(s, c))
	{
		while (((char *)s)[i] == c && ((char *)s)[i])
			i++;
		start = i;
		while (((char *)s)[i] != c && ((char *)s)[i])
			i++;
		res[row] = malloc(i - start + 1);
		if (!res[row])
			return (free_if_error(res));
		str_fill(res[row], s + start, i - start);
		row++;
	}
	res[row] = NULL;
	return (res);
}
