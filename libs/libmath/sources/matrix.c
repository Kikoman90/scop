/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 19:23:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/09 17:45:43 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_mat4x4	quat_to_mat4x4(t_quaternion q)
{
	t_mat4x4	mat;
	float xx;
	float yy;
	float zz;

	mat = (t_mat4x4)IDENTITY_MATRIX4;
	xx = q.x * q.x;
	yy = q.y * q.y;
	zz = q.z * q.z;
	mat.m[0] = 1 - 2 * (yy + zz);
	mat.m[1] = 2 * (q.x * q.y + q.z * q.w);
	mat.m[2] = 2 * (q.x * q.z - q.y * q.w);
	mat.m[4] = 2 * (q.x * q.y - q.z * q.w);
	mat.m[5] = 1 - 2 * (xx + zz);
	mat.m[6] = 2 * (q.y * q.z + q.x * q.w);
	mat.m[8] = 2 * (q.x * q.z + q.y * q.w);
	mat.m[9] = 2 * (q.y * q.z - q.x * q.w);
	mat.m[10] = 1 - 2 * (xx + yy);
	return (mat);
}

t_mat4x4	mat4x4(void)
{
	return ((t_mat4x4)IDENTITY_MATRIX4);
}

t_mat4x4	mat4x4_trs(t_vec3 t, t_quaternion r, t_vec3 s) //quaternion r ?
{
	t_mat4x4	mat;
	float xx;
	float yy;
	float zz;

	mat = (t_mat4x4)IDENTITY_MATRIX4;
	xx = r.x * r.x;
	yy = r.y * r.y;
	zz = r.z * r.z;
	mat.m[0] = s.x - (2 * s.x * (yy + zz));
	mat.m[1] = 2 * s.y * (r.x * r.y - r.z * r.w);
	mat.m[2] = 2 * s.z * (r.x * r.z + r.y * r.w);
	mat.m[3] = t.x;
	mat.m[4] = 2 * s.x * (r.x * r.y + r.z * r.w);
	mat.m[5] = s.y - (2 * s.y * (xx + zz));
	mat.m[6] = 2 * s.z * (r.y * r.z - r.x * r.w);
	mat.m[7] = t.y;
	mat.m[8] = 2 * s.x * (r.x * r.z - r.y * r.w);
	mat.m[9] = 2 * s.y * (r.y * r.z + r.x * r.w);
	mat.m[10] = s.z - (2 * s.z * (xx + yy));
	mat.m[11] = t.z;
	
	return (mat);
	/* // mat = mat4x4_scale(mat, s);
	mat.m[0] = s.x;
	mat.m[5] = s.y;
	mat.m[10] = s.z;

	// mat = mat4x4_rotate(mat, r)


	// mat = mat4x4_translate(mat, t);
	mat.m[3] = t.x;
	mat.m[7] = t.y;
	mat.m[11] = t.z;*/
}

/*t_mat4x4    mat_init(t_mat4x4 mat, float f)
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
}*/

/*t_vec3		mat4x4_t(t_mat4x4 mat)
{
	t_vec3	t;

	t.x = mat.m[3];
	t.y = mat.m[7];
	t.z = mat.m[11];
	return (t);
}*/

t_mat4x4	mat4x4_translate(t_vec3 t)
{
	t_mat4x4	mat;

	mat.m[3] = t.x;
	mat.m[7] = t.y;
	mat.m[11] = t.z;
	return (mat);
}

t_mat4x4	mat4x4_scale(t_vec3 s)
{
	t_mat4x4	mat;

	mat.m[0] = s.x;
	mat.m[5] = s.y;
	mat.m[10] = s.z;
	return (mat);
}

t_mat4x4    mat4x4_add(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.m[i] += b.m[i];
	return (a);
}

t_mat4x4    mat4x4_sub(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.m[i] -= b.m[i];
	return (a);
}

static void	swapf(float *a, float *b)
{
	float c;

	c = *a;
	*a = *b;
	*b = c;
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

t_vec4		vec4_mat4x4_prod(t_vec4 v, t_mat4x4 mat)
{
	unsigned int	i;
	unsigned int	j;
	t_vec4			res;

	j = 0;
	while (j < 4)
	{
		i = 0;
		res.v[j] = 0;
		while (i < 4)
		{
			res.v[j] += v.v[i] * mat.m[i * 4 + j];
			i++;
		}
		j++;
	}
	return (res);
}

t_vec3		vec3_mat4x4_prod(t_vec3 v, t_mat4x4 mat)
{
	t_vec4	v4;

	v4 = vec4_mat4x4_prod(vec4_v3w(v, 1), mat);
	return (vec3_xyz(v4.x, v4.y, v4.z));
}

t_mat4x4    mat4x4_mult(t_mat4x4 a, t_mat4x4 b)
{
	t_mat4x4	res;

	res.v[0] = vec4_mat4x4_prod(a.v[0], b);
	res.v[1] = vec4_mat4x4_prod(a.v[0], b);
	res.v[2] = vec4_mat4x4_prod(a.v[0], b);
	res.v[3] = vec4_mat4x4_prod(a.v[0], b);
	return (res);
}
