/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 15:51:02 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/04 15:51:03 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int     fileSize(int fd) {
   struct stat  s;

   if (fstat(fd, &s) == -1)
   {
       char *errorMsg = ft_strjoin("error: ", strerror(errno));
       ft_putendl(errorMsg);
       free(errorMsg);
       return(-1);
   }
   return(s.st_size);
}

void    *LogErrorNull(const char *msg)
{
    printf("ERROR: %s\n", msg);
    return (NULL);
}

void    LogError(const char *msg)
{
    printf("ERROR: %s\n", msg);
}

/* gameObject =
 * rendering properties
 * physics properties
 * audio properties
 * behaviour(s) */