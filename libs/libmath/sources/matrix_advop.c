/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_advop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 11:31:56 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/29 14:04:59 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_vec3			vec3_mat4x4_prod(t_mat4x4 mat, t_vec3 v, float w)
{
	t_vec4	v4;

	v4 = vec4_mat4x4_prod(mat, vec4_v3w(v, w));
	return (vec3_xyz(v4.x, v4.y, v4.z));
}

t_vec4			vec4_mat4x4_prod(t_mat4x4 mat, t_vec4 v)
{
	t_vec4	res;

	res.x = v.x * mat.m[0] + v.y * mat.m[4] + v.z * mat.m[8] + v.w * mat.m[12];
	res.y = v.x * mat.m[1] + v.y * mat.m[5] + v.z * mat.m[9] + v.w * mat.m[13];
	res.z = v.x * mat.m[2] + v.y * mat.m[6] + v.z * mat.m[10] + v.w * mat.m[14];
	res.w = v.x * mat.m[3] + v.y * mat.m[7] + v.z * mat.m[11] + v.w * mat.m[15];
	return (res);
}

t_mat4x4		mat4x4_mult(t_mat4x4 a, t_mat4x4 b)
{
	t_mat4x4	res;

	res.v[0] = vec4_mat4x4_prod(a, b.v[0]);
	res.v[1] = vec4_mat4x4_prod(a, b.v[1]);
	res.v[2] = vec4_mat4x4_prod(a, b.v[2]);
	res.v[3] = vec4_mat4x4_prod(a, b.v[3]);
	return (res);
}

t_mat4x4		quat_to_mat4x4(t_quaternion q)
{
	t_mat4x4	mat;
	float		xx;
	float		yy;
	float		zz;

	mat = mat4x4();
	xx = q.v.x * q.v.x;
	yy = q.v.y * q.v.y;
	zz = q.v.z * q.v.z;
	mat.m[0] = 1 - 2 * (yy + zz);
	mat.m[1] = 2 * (q.v.x * q.v.y - q.v.z * q.w);
	mat.m[2] = 2 * (q.v.x * q.v.z + q.v.y * q.w);
	mat.m[4] = 2 * (q.v.x * q.v.y + q.v.z * q.w);
	mat.m[5] = 1 - 2 * (xx + zz);
	mat.m[6] = 2 * (q.v.y * q.v.z - q.v.x * q.w);
	mat.m[8] = 2 * (q.v.x * q.v.z - q.v.y * q.w);
	mat.m[9] = 2 * (q.v.y * q.v.z + q.v.x * q.w);
	mat.m[10] = 1 - 2 * (xx + yy);
	return (mat);
}
