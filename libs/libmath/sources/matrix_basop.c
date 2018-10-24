/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_basop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 11:32:03 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/23 17:59:01 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

static void	swapf(float *a, float *b)
{
	float c;

	c = *a;
	*a = *b;
	*b = c;
}

t_mat4x4	mat4x4_add(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.m[i] += b.m[i];
	return (a);
}

t_mat4x4	mat4x4_sub(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.m[i] -= b.m[i];
	return (a);
}

t_mat4x4	mat4x4_transpose(t_mat4x4 mat)
{
	swapf(&mat.m[1], &mat.m[4]);
	swapf(&mat.m[2], &mat.m[8]);
	swapf(&mat.m[3], &mat.m[12]);
	swapf(&mat.m[6], &mat.m[9]);
	swapf(&mat.m[7], &mat.m[13]);
	swapf(&mat.m[11], &mat.m[14]);
	return (mat);
}
