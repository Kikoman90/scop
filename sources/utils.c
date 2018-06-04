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

void*   LogErrorNull(const char *msg)
{
    printf("ERROR: %s\n", msg);
    return (NULL);    
}

void    LogError(const char *msg)
{
    printf("ERROR: %s\n", msg);
}