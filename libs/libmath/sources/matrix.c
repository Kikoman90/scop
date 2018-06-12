/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 19:23:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/06/12 19:23:54 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_mat4x4    matInit(t_mat4x4 *mat, float f)
{
    int		i;

	i = -1;
	while (++i < 16)
	{
		if (f == MAT_ID)
			mat->m[i] = (i % 5 == 0 ? 1 : 0);
		else
			mat->m[i] = f;
	}
}

t_mat4x4    mat4x4Add(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.m[i] += b.m[i];
	return (a);
}

t_mat4x4    mat4x4Sub(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.m[i] -= b.m[i];
	return (a);
}

t_mat4x4    matMult(t_mat4x4 a, t_mat4x4 b)
{
    
}
