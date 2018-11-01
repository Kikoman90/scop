/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 14:26:55 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/01 21:42:14 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_file_map(const char *path, size_t *data_size)
{
	int			fd;
	char		*data;
	struct stat	file_stat;

	if ((fd = open(path, O_RDWR)) == -1)
	{
		log_error_free(ft_strjoin("(open) ", strerror(errno)));
		return (NULL);
	}
	if (fstat(fd, &file_stat) == -1)
	{
		close(fd);
		log_error_free(ft_strjoin("(fstat) ", strerror(errno)));
		return (NULL);
	}
	*data_size = (size_t)file_stat.st_size;
	if ((data = (char*)mmap(NULL, *data_size, \
		PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		log_error_free(ft_strjoin("(mmap) ", strerror(errno)));
		close(fd);
		return (NULL);
	}
	close(fd);
	return (data);
}

void	ft_file_unmap(char *data, size_t fsize, char *fpath)
{
	if (fpath)
	{
		free(fpath);
		fpath = NULL;
	}
	if (data)
	{
		if (munmap(data, fsize) == -1)
			log_error_free(ft_strjoin("(munmap) ", strerror(errno)));
		data = NULL;
	}
}
