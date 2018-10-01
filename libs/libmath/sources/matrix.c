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

t_mat4x4	quat_to_mat4x4(t_quaternion q)
{
	/*xx      = X * X;
    xy      = X * Y;
    xz      = X * Z;
    xw      = X * W;
    yy      = Y * Y;
    yz      = Y * Z;
    yw      = Y * W;
    zz      = Z * Z;
    zw      = Z * W;
    mat[0]  = 1 - 2 * ( yy + zz );
    mat[1]  =     2 * ( xy - zw );
    mat[2]  =     2 * ( xz + yw );
    mat[4]  =     2 * ( xy + zw );
    mat[5]  = 1 - 2 * ( xx + zz );
    mat[6]  =     2 * ( yz - xw );
    mat[8]  =     2 * ( xz - yw );
    mat[9]  =     2 * ( yz + xw );
    mat[10] = 1 - 2 * ( xx + yy );
    mat[3]  = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
    mat[15] = 1;

  The resulting matrix uses the following positions:

      � mat[0]  mat[4] mat[ 8] mat[12] �
  M = � mat[1]  mat[5] mat[ 9] mat[13] �
      � mat[2]  mat[6] mat[10] mat[14] �
      � mat[3]  mat[7] mat[11] mat[15] �*/

	t_mat4x4	mat;

	mat = mat4x4_init();
	mat.m[0] = 1 - (2 * (sqrtf(q.v.y) + sqrtf(q.v.z)));
	mat.m[1] = 2 * (q.v.x * q.v.y - q.v.z * q.w);
	mat.m[2] = 2 * (q.v.x * q.v.z + q.v.y * q.w);
	mat.m[4] = 2 * (q.v.x * q.v.y + q.v.z * q.w);
	mat.m[5] = 1 - (2 * (sqrtf(q.v.x) + sqrtf(q.v.z)));
	mat.m[6] = 2 * (q.v.y * q.v.z - q.v.x * q.w);
	mat.m[8] = 2 * (q.v.x * q.v.z - q.v.y * q.w);
	mat.m[9] = 2 * (q.v.y * q.v.z + q.v.x * q.w);
	mat.m[10] = 1 - (2 * (sqrtf(q.v.x) + sqrtf(q.v.y)));
	return (mat);
}

t_mat4x4	mat4x4_init()
{
	return ((t_mat4x4)IDENTITY_MATRIX4);
}

t_mat4x4	mat4x4_init_trs(t_vec3 t, t_vec3 r, t_vec3 s) //quaternion r ?
{
	t_mat4x4	mat;

	mat = IDENTITY_MATRIX4;
	mat.m[3] = t.x;
	mat.m[7] = t.y;
	mat.m[11] = t.z;
	//...
	return (mat);
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

t_vec3		mat4x4_t(t_mat4x4 mat)
{
	t_vec3	t;

	t.x = mat[3];
	t.y = mat[7];
	t.z = mat[11];
	return (t);
}

t_mat4x4    mat4x4_add(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.mat.m[i] += b.mat.m[i];
	return (a);
}

t_mat4x4    mat4x4_sub(t_mat4x4 a, t_mat4x4 b)
{
	int		i;

	i = -1;
	while (++i < 16)
		a.mat.m[i] -= b.mat.m[i];
	return (a);
}

t_mat4x4	mat4x4_translate(t_mat4x4 mat, t_vec3 t)
{
	mat.m[3] = t.x;
	mat.m[7] = t.y;
	mat.m[11] = t.z;
	return (mat);
}

t_mat4x4	mat4x4_transpose(t_mat4x4 mat)
{
	float keeper;

	keeper = mat.m[1];
	mat.m[1] = mat.m[4];
	mat.m[4] = keeper;
	keeper = mat.m[2];
	mat.m[2] = mat.m[8];
	mat.m[8] = keeper;
	keeper = mat.m[3];
	mat.m[3] = mat.m[12];
	mat.m[12] = keeper;
	keeper = mat.m[6]
	mat.m[6] = mat.m[9];
	mat.m[9] = keeper;
	keeper = mat.m[7];
	mat.m[7] = mat.m[13];
	mat.m[13] = keeper;
	keeper = mat.m[11];
	mat.m[11] = mat.m[14];
	mat.m[14] = keeper;
	return (mat);
}

/*t_mat4x4    mat_mult(t_mat4x4 a, t_mat4x4 b)
{
    
}*/
