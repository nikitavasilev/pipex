/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 01:03:40 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/14 06:54:21 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	if (start > s_len)
		s_len = 0;
	if (s_len > len)
		s_len = len;
	str = malloc(s_len + 1);
	if (!str)
		return (0);
	i = 0;
	if (s_len)
	{
		while (s[start] && i < len)
			str[i++] = s[start++];
	}
	str[i] = '\0';
	return (str);
}
