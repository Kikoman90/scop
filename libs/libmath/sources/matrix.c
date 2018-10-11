/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 19:23:53 by fsidler           #+#    #+#             */
/*   Updated: 2018/10/11 17:17:59 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmath.h"

t_mat4x4	mat4x4(void)
{
	return ((t_mat4x4)IDENTITY_MATRIX4);
}

t_mat4x4	mat4x4_trs(t_vec3 t, t_quaternion r, t_vec3 s)
{
	t_mat4x4	trs;
	t_mat4x4	rot_mat;

	rot_mat = quat_to_mat4x4(r);
	trs = (t_mat4x4)IDENTITY_MATRIX4;
	trs.m[0] = s.x * rot_mat.m[0];
	trs.m[1] = s.x * rot_mat.m[1];
	trs.m[2] = s.x * rot_mat.m[2];
	trs.m[12] = t.x;
	trs.m[4] = s.y * rot_mat.m[4];
	trs.m[5] = s.y * rot_mat.m[5];
	trs.m[6] = s.y * rot_mat.m[6];
	trs.m[13] = t.y;
	trs.m[8] = s.z * rot_mat.m[8];
	trs.m[9] = s.z * rot_mat.m[9];
	trs.m[10] = s.z * rot_mat.m[10];
	trs.m[14] = t.z;
	return (trs);
}