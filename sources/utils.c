/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:51:02 by fsidler           #+#    #+#             */
/*   Updated: 2018/09/12 21:23:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

const char		*file_name(const char *path)
{
	int		i;
	int		j;
	char	*name;

	i = ft_strlen(path) - 1;
	j = 0;
	while (path[i] && path[i] != '/')
		i--;
	i++;
	name = ft_strnew(ft_strlen(path) - i);
	while (path[i])
		name[j++] = path[i++];
	return (name);
}

int				file_size(int fd)
{
	struct stat	s;

	if (fstat(fd, &s) == -1)
	{
		log_error(strerror(errno));
		return (-1);
	}
	return (s.st_size);
}