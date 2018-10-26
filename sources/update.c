/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:43:49 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/26 16:17:40 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

unsigned int    mat_update(t_mat4x4 **mat, size_t count)
{
    if (count == 0)
    {
        *mat = NULL;
        return (0);
    }
    if (*mat)
        free (*mat);
    count++;
    if (!(*mat = (t_mat4x4*)malloc(sizeof(t_mat4x4) * count)))
        return (1);
    printf("i don't get it yet\n");
    return (0);
}