/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 19:23:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/11/21 17:36:12 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"
#include <stdio.h>//remove me

t_mat4x4	mat4x4(void)
{
	return ((t_mat4x4){{1.0f, 0.0f, 0.0f, 0.0f,\
						0.0f, 1.0f, 0.0f, 0.0f,\
						0.0f, 0.0f, 1.0f, 0.0f,\
						0.0f, 0.0f, 0.0f, 1.0f}});
}

t_mat4x4	mat4x4_trs(t_vec3 t, t_quaternion r, t_vec3 s)
{
	t_mat4x4	trs;
	t_mat4x4	rot_mat;

	rot_mat = quat_to_mat4x4(r);
	trs = mat4x4();
	trs.m[0] = s.x * rot_mat.m[0];
	trs.m[1] = s.x * rot_mat.m[1];
	trs.m[2] = s.x * rot_mat.m[2];
	trs.m[4] = s.y * rot_mat.m[4];
	trs.m[5] = s.y * rot_mat.m[5];
	trs.m[6] = s.y * rot_mat.m[6];
	trs.m[8] = s.z * rot_mat.m[8];
	trs.m[9] = s.z * rot_mat.m[9];
	trs.m[10] = s.z * rot_mat.m[10];
	trs.m[12] = t.x;
	trs.m[13] = t.y;
	trs.m[14] = t.z;
	return (trs);
}

t_mat4x4	compute_view(t_transform cam_transform)
{
	t_mat4x4	view;

	view = mat4x4_transpose(quat_to_mat4x4(cam_transform.rotation));
	view.m[3] = -vec3_dot(vec3_v4(view.v[0]), cam_transform.position);
	view.m[7] = -vec3_dot(vec3_v4(view.v[1]), cam_transform.position);
	view.m[11] = -vec3_dot(vec3_v4(view.v[2]), cam_transform.position);
	return (mat4x4_transpose(view));
}

// remove me
t_mat4x4	compute_orthographic(float bounds[6], float aspect)
{
	t_mat4x4	ortho_mat;

	(void)aspect;
	ortho_mat = mat4x4();
	/*if (aspect >= 1)
	{
		bounds[0] *= aspect;
		bounds[1] *= aspect;
	}
	else
	{
		bounds[2] *= aspect;
		bounds[3] *= aspect;
	}*/
	ortho_mat.m[0] = 2 / (bounds[1] - bounds[0]);
	ortho_mat.m[5] = 2 / (bounds[3] - bounds[2]);
	ortho_mat.m[10] = -2 / (bounds[5] - bounds[4]);
	ortho_mat.m[12] = -(bounds[1] + bounds[0]) / (bounds[1] - bounds[0]);
	ortho_mat.m[13] = -(bounds[3] + bounds[2]) / (bounds[3] - bounds[2]);
	ortho_mat.m[14] = -(bounds[5] + bounds[4]) / (bounds[5] - bounds[4]);
	return (ortho_mat);
}

t_mat4x4	compute_perspective(float fov, float aspect, float zn, float zf)
{
	t_mat4x4	proj_mat;
	float		f;

	fov *= M_PI / 180.0f;
	f = 1 / tanf(fov / 2);
	proj_mat = mat4x4();
	proj_mat.m[0] = f / aspect;
	proj_mat.m[5] = f;
	proj_mat.m[10] = -(zf + zn) / (zf - zn);
	proj_mat.m[11] = -1;
	proj_mat.m[14] = (-2 * zn * zf) / (zf - zn);
	proj_mat.m[15] = 0;
	return (proj_mat);
}
