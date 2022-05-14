/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_dir.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvasilev <nvasilev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 17:37:18 by nvasilev          #+#    #+#             */
/*   Updated: 2022/05/12 02:20:16 by nvasilev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int	is_dir(char *path)
{
	int	fd;

	if (!path)
	{
		errno = EINVAL;
		return (errno);
	}
	fd = open(path, O_RDWR);
	if ((fd == -1) && (errno == EISDIR))
	{
		errno = EISDIR;
		return (errno);
	}
	if (fd > 0)
		close(fd);
	return (0);
}
