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

t_mat4x4    mat_init_f(t_mat4x4 mat, float f)
{
    int		i;

	i = -1;
	while (++i < 16)
	{
		if (f == MAT_ID)
			mat.mat.m[i] = (i % 5 == 0 ? 1 : 0);
		else
			mat.mat.m[i] = f;
	}
	return (mat);
}

t_mat4x4    mat_add(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.mat.m[i] += b.mat.m[i];
	return (a);
}

t_mat4x4    mat_sub(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.mat.m[i] -= b.mat.m[i];
	return (a);
}

/*t_mat4x4    mat_mult(t_mat4x4 a, t_mat4x4 b)
{
    
}*/
