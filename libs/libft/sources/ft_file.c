/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/01 14:26:55 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/15 14:18:53 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_file_map(const char *path, size_t *data_size)
{
	int			fd;
	void		*data;
	struct stat	file_stat;

	if ((fd = open(path, O_RDWR)) == -1)
		return (log_error_null_free(ft_strjoin("(open) ", strerror(errno))));
	if (fstat(fd, &file_stat) == -1)
	{
		close(fd);
		return (log_error_null_free(ft_strjoin("(fstat) ", strerror(errno))));
	}
	*data_size = (size_t)file_stat.st_size;
	if ((data = mmap(NULL, *data_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, \
		fd, 0)) == MAP_FAILED)
	{
		close(fd);
		return (log_error_null_free(ft_strjoin("(mmap) ", strerror(errno))));
	}
	close(fd);
	return (data);
}

void	ft_file_unmap(void *data, size_t fsize, char *fpath)
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
