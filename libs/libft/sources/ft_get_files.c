/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 16:37:58 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/20 10:56:34 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void        ft_free_file_names(char **names, unsigned int nb)
{
    while (nb--)
    {
        free(names[nb]);
        names[nb] = NULL;
    }   
    free(names);
    names = NULL;
}

static int	str_compare(const void *a, const void *b)
{
	const char * const *pa = a;
	const char * const *pb = b;
	return (ft_strcmp(*pa, *pb));
}

char		**ft_get_file_names(const char *dir_path, unsigned int nb, int sort)
{
	DIR				*dir;
	struct dirent	*dp;
	char			**names;
    unsigned int    i;
	
	if (!(names = (char**)malloc(sizeof(char*) * nb)))
		return (log_error_null(MALLOC_ERROR));
	if (!(dir = opendir(dir_path)))
	{
		free(names);
		return (log_error_null_free(ft_strjoin("(opendir) ", strerror(errno))));
	}
    i = 0;
	while (i < nb && (dp = readdir(dir)))
	{
		if (dp->d_name[0] != '.')
			names[i++] = ft_strdup(dp->d_name);
	}
	if (closedir(dir) && i != nb)
	{
		ft_free_file_names(names, nb);
        return (log_error_null_free(ft_strjoin(MISSING_FILE_ERROR, dir_path)));
	}
	(sort) ? qsort(names, nb, sizeof(char*), str_compare) : 0;
	return (names);
}